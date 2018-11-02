//
// _SCParser_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCParser.h"

LLVMContext SCParser::TheContext;
IRBuilder<> SCParser::Builder(TheContext);
std::unique_ptr<Module> SCParser::TheModule;
std::unique_ptr<legacy::FunctionPassManager> SCParser::TheFPM;
std::map<std::string, Value *> SCParser::NamedValues;

SCParser::SCParser(std::string B, std::string F, SCMsg *M)
  : CurTok(-1), InBuf(B), FileName(F), Msgs(M), Lex(new SCLexer(B)),
    InFunc(false), IsOpt(false) {
  TheModule = llvm::make_unique<Module>(StringRef(FileName), TheContext);
  InitBinopPrecedence();
}

SCParser::SCParser(SCMsg *M)
  : CurTok(-1), Msgs(M), Lex(nullptr),
    InFunc(false), IsOpt(false) {
  InitBinopPrecedence();
}

SCParser::~SCParser(){
  TheModule.reset();
  TheFPM.reset();
  NamedValues.clear();
}

bool SCParser::SetInputs( std::string B, std::string F ){
  InBuf = B;
  FileName = F;
  if( Lex ){
    delete Lex;
  }
  Lex = new SCLexer(InBuf);
  TheModule = llvm::make_unique<Module>(StringRef(FileName), TheContext);

  return true;
}

//===----------------------------------------------------------------------===//
// Optimizer
//===----------------------------------------------------------------------===//

void SCParser::InitModuleandPassManager(){
  // create a new pass manager
  TheFPM = llvm::make_unique<legacy::FunctionPassManager>(TheModule.get());

  // enable simple peephole opts and bit-twiddling opts
  TheFPM->add(createInstructionCombiningPass());

  // enable reassociatio of epxressions
  TheFPM->add(createReassociatePass());

  // eliminate common subexpressions
  TheFPM->add(createGVNPass());

  // simplify the control flow graph
  TheFPM->add(createCFGSimplificationPass());

  // constant propogations
  TheFPM->add(createConstantPropagationPass());

  // induction variable simplification pass
  TheFPM->add(createIndVarSimplifyPass());

  // loop invariant code motion
  TheFPM->add(createLICMPass());

  // loop deletion
  TheFPM->add(createLoopDeletionPass());

  // loop idiom
  TheFPM->add(createLoopIdiomPass());

  // loop re-roller
  TheFPM->add(createLoopRerollPass());

  // loop rotation
  TheFPM->add(createLoopRotatePass());

  // loop unswitching
  TheFPM->add(createLoopUnswitchPass());

  // Init it!
  TheFPM->doInitialization();
}

bool SCParser::Optimize(){
  InitModuleandPassManager();
  IsOpt = true;
  return true;
}

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

void SCParser::InitBinopPrecedence(){
  BinopPrecedence['='] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;
  BinopPrecedence['/'] = 40;
  BinopPrecedence['%'] = 40;
#if 0
  BinopPrecedence["="] = 10;
  BinopPrecedence["+"] = 20;
  BinopPrecedence["-"] = 20;
  BinopPrecedence["*"] = 40;
  BinopPrecedence["/"] = 40;
  BinopPrecedence["%"] = 40;
  BinopPrecedence["|"] = 20;
  BinopPrecedence["&"] = 20;
  BinopPrecedence["^"] = 20;
  BinopPrecedence["~"] = 20;
  BinopPrecedence["!"] = 50;
  BinopPrecedence["<"] = 10;
  BinopPrecedence[">"] = 10;

  BinopPrecedence["!="] = 10;
  BinopPrecedence["<<"] = 10;
  BinopPrecedence[">>"] = 10;
  BinopPrecedence["||"] = 10;
  BinopPrecedence["&&"] = 10;
  BinopPrecedence[">="] = 10;
  BinopPrecedence["<="] = 10;
  BinopPrecedence["=="] = 10;
#endif
}

bool SCParser::Parse(){
  if( InBuf.length() == 0 ){
    Msgs->PrintMsg( L_ERROR, "No StoneCutter input defined");
    return false;
  }

  // pull the first token
  GetNextToken();

  while(true){
    switch(CurTok){
    case tok_eof:
      return true;
    case ';':
      GetNextToken();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    case tok_regclass:
      HandleRegClass();
      break;
    case '}':
      HandleFuncClose();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }

  return true;
}

int SCParser::GetNextToken(){
  return CurTok = Lex->GetTok();
}

int SCParser::GetTokPrecedence(){
  if (!isascii(CurTok))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

std::unique_ptr<ExprAST> SCParser::ParseNumberExpr() {
  auto Result = llvm::make_unique<NumberExprAST>(Lex->GetNumVal());
  GetNextToken(); // consume the number
  return std::move(Result);
}

std::unique_ptr<ExprAST> SCParser::ParseParenExpr() {
  GetNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')'");
  GetNextToken(); // eat ).
  return V;
}

std::unique_ptr<ExprAST> SCParser::ParseIdentifierExpr() {
  std::string IdName = Lex->GetIdentifierStr();

  GetNextToken(); // eat identifier.

  if (CurTok != '(') // Simple variable ref.
    return llvm::make_unique<VariableExprAST>(IdName);

  // Call.
  GetNextToken(); // eat (
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (CurTok != ')') {
    while (true) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (CurTok == ')')
        break;

      if (CurTok != ',')
        return LogError("Expected ')' or ',' in argument list");
      GetNextToken();
    }
  }

  // Eat the ')'.
  GetNextToken();

  return llvm::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> SCParser::ParseIfExpr() {
  GetNextToken();  // eat the if.

  // condition.
  auto Cond = ParseExpression();
  if (!Cond)
    return nullptr;

  if (CurTok != tok_then)
    return LogError("expected then");
  GetNextToken();  // eat the then

  auto Then = ParseExpression();
  if (!Then)
    return nullptr;

  if (CurTok != tok_else)
    return LogError("expected else");

  GetNextToken();

  auto Else = ParseExpression();
  if (!Else)
    return nullptr;

  return llvm::make_unique<IfExprAST>(std::move(Cond), std::move(Then),
                                      std::move(Else));
}

std::unique_ptr<ExprAST> SCParser::ParsePrimary() {
  switch (CurTok) {
  default:
    return LogError("unknown token when expecting an expression");
  case tok_identifier:
    return ParseIdentifierExpr();
  case tok_number:
    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  case tok_if:
    return ParseIfExpr();
  }
}

std::unique_ptr<ExprAST> SCParser::ParseBinOpRHS(int ExprPrec,
                                                 std::unique_ptr<ExprAST> LHS) {
  // If this is a binop, find its precedence.
  while (true) {
    int TokPrec = GetTokPrecedence();

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = CurTok;
    GetNextToken(); // eat binop

    // Parse the primary expression after the binary operator.
    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS = llvm::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                           std::move(RHS));
  }
}

std::unique_ptr<ExprAST> SCParser::ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<PrototypeAST> SCParser::ParsePrototype() {
  if (CurTok != tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = Lex->GetIdentifierStr();
  GetNextToken();

  if (CurTok != '(')
    return LogErrorP("Expected '(' in prototype");

  std::vector<std::string> ArgNames;
  while (GetNextToken() == tok_identifier)
    ArgNames.push_back(Lex->GetIdentifierStr());
  if (CurTok != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  GetNextToken(); // eat ')'.

  return llvm::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<RegClassAST> SCParser::ParseRegClassDef(){
  if (CurTok != tok_identifier)
    return LogErrorR("Expected regclass name in prototype");

  std::string RName = Lex->GetIdentifierStr();
  GetNextToken();

  if (CurTok != '(')
    return LogErrorR("Expected '(' in regclass prototype");

  std::vector<std::string> ArgNames;
  while (GetNextToken() == tok_identifier)
    ArgNames.push_back(Lex->GetIdentifierStr());
  if (CurTok != ')')
    return LogErrorR("Expected ')' in regclass prototype");

  // success.
  GetNextToken(); // eat ')'.

  return llvm::make_unique<RegClassAST>(RName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> SCParser::ParseDefinition() {
  GetNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  // check for open instruction body
  if( CurTok != '{' )
    return LogErrorF("Expected '{' to open instruction body");
  if( InFunc ){
    return LogErrorF("Found mismatch '{'");
  }
  GetNextToken(); // eat the '{'
  InFunc = true;

  if (auto E = ParseExpression())
    return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));

  return nullptr;
}

bool SCParser::ParseCloseBracket(){
  if( !InFunc ){
    LogErrorF("Found mismatched '}'");
    return false;
  }
  InFunc = false;
  GetNextToken(); // get the next token
  return true;
}

std::unique_ptr<RegClassAST> SCParser::ParseRegClass(){
  GetNextToken(); //eat regclass
  return ParseRegClassDef();
}

std::unique_ptr<FunctionAST> SCParser::ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    // Make an anonymous proto.
    auto Proto = llvm::make_unique<PrototypeAST>("__anon_expr",
                                                 std::vector<std::string>());
    return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

std::unique_ptr<PrototypeAST> SCParser::ParseExtern(){
  GetNextToken(); // eat extern.
  return ParsePrototype();
}

void SCParser::HandleDefinition() {
  if (auto FnAST = ParseDefinition()) {
    if (auto *FnIR = FnAST->codegen()) {
      //fprintf(stderr, "Read function definition:");
      //FnIR->print(errs());
      //fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    GetNextToken();
  }
}

void SCParser::HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      //fprintf(stderr, "Read extern: ");
      //FnIR->print(errs());
      //fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    GetNextToken();
  }
}

void SCParser::HandleRegClass() {
  // evaluate a register class definition
  if(auto RegClassAST = ParseRegClass()){
    if( auto *FnIR = RegClassAST->codegen()){
      //fprintf(stderr, "Read register class definition");
      //FnIR->print(errs());
      //fprintf(stderr, "\n");
    }
  }else{
    GetNextToken();
  }
}

void SCParser::HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto FnAST = ParseTopLevelExpr()) {
    if (auto *FnIR = FnAST->codegen()) {
      //fprintf(stderr, "Read top-level expression:");
      //FnIR->print(errs());
      //fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    GetNextToken();
  }
}

void SCParser::HandleFuncClose(){
  if(ParseCloseBracket()){
    //fprintf(stderr, "Parsed a function body\n");
  }else{
    GetNextToken();
  }
}

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//
Value *NumberExprAST::codegen() {
  return ConstantFP::get(SCParser::TheContext, APFloat(Val));
}

Value *VariableExprAST::codegen() {
  // Look this variable up in the function.
  Value *V = SCParser::NamedValues[Name];
  if (!V)
    return LogErrorV("Unknown variable name");
  return V;
}

Value *BinaryExprAST::codegen() {
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op) {
  case '+':
    return SCParser::Builder.CreateFAdd(L, R, "addtmp");
  case '-':
    return SCParser::Builder.CreateFSub(L, R, "subtmp");
  case '*':
    return SCParser::Builder.CreateFMul(L, R, "multmp");
  case '<':
    L = SCParser::Builder.CreateFCmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return SCParser::Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

Value *CallExprAST::codegen() {
  // Look up the name in the global module table.
  Function *CalleeF = SCParser::TheModule->getFunction(Callee);
  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  // If argument mismatch error.
  if (CalleeF->arg_size() != Args.size())
    return LogErrorV("Incorrect # arguments passed");

  std::vector<Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }

  return SCParser::Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *PrototypeAST::codegen() {
  // Make the function type:  double(double,double) etc.
  std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(SCParser::TheContext));
  FunctionType *FT =
      FunctionType::get(Type::getDoubleTy(SCParser::TheContext), Doubles, false);

  Function *F =
      Function::Create(FT, Function::ExternalLinkage, Name, SCParser::TheModule.get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

Value *RegClassAST::codegen(){
  return nullptr;
}

Function *FunctionAST::codegen() {
  // First, check for an existing function from a previous 'extern' declaration.
  Function *TheFunction = SCParser::TheModule->getFunction(Proto->getName());

  if (!TheFunction)
    TheFunction = Proto->codegen();

  if (!TheFunction)
    return nullptr;

  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(SCParser::TheContext, "entry", TheFunction);
  SCParser::Builder.SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  SCParser::NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    SCParser::NamedValues[Arg.getName()] = &Arg;

  if (Value *RetVal = Body->codegen()) {
    // Finish off the function.
    SCParser::Builder.CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);

    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return nullptr;
}

Value *IfExprAST::codegen() {
  Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;

  // Convert condition to a bool by comparing non-equal to 0.0.
  CondV = Builder.CreateFCmpONE(
      CondV, ConstantFP::get(TheContext, APFloat(0.0)), "ifcond");

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  // Emit then value.
  Builder.SetInsertPoint(ThenBB);

  Value *ThenV = Then->codegen();
  if (!ThenV)
    return nullptr;

  Builder.CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = Builder.GetInsertBlock();

  // Emit else block.
  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);

  Value *ElseV = Else->codegen();
  if (!ElseV)
    return nullptr;

  Builder.CreateBr(MergeBB);
  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = Builder.GetInsertBlock();

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  PHINode *PN = Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, "iftmp");

  PN->addIncoming(ThenV, ThenBB);
  PN->addIncoming(ElseV, ElseBB);
  return PN;
  return nullptr;
}

//===----------------------------------------------------------------------===//
// Error Handlers
//===----------------------------------------------------------------------===//

std::unique_ptr<ExprAST> SCParser::LogError(std::string Str) {
  Msgs->PrintMsg( L_ERROR, "Line " + std::to_string(Lex->GetLineNum()) + " : " + Str );
  return nullptr;
}

std::unique_ptr<PrototypeAST> SCParser::LogErrorP(std::string Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<RegClassAST> SCParser::LogErrorR(std::string Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<FunctionAST> SCParser::LogErrorF(std::string Str) {
  LogError(Str);
  return nullptr;
}

Value *LogErrorV(std::string Str){
  //LogError(Str);
  // TODO, fix error handling
  return nullptr;
}

// EOF
