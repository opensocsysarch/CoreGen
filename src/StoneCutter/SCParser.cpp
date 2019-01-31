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
std::map<std::string, std::unique_ptr<PrototypeAST>> SCParser::FunctionProtos;
std::map<std::string, AllocaInst*> SCParser::NamedValues;
std::map<std::string, GlobalVariable*> SCParser::GlobalNamedValues;
unsigned SCParser::LabelIncr;
bool SCParser::IsOpt = false;
SCMsg *SCParser::GMsgs = nullptr;

SCParser::SCParser(std::string B, std::string F, SCMsg *M)
  : CurTok(-1), InBuf(B), FileName(F), Msgs(M), Lex(new SCLexer()),
    InFunc(false), Rtn(true) {
  TheModule = llvm::make_unique<Module>(StringRef(FileName), TheContext);
  GMsgs = M;
  InitBinopPrecedence();
  LabelIncr = 0;
  InitPassMap();
  InitIntrinsics();
}

SCParser::SCParser(SCMsg *M)
  : CurTok(-1), Msgs(M), Lex(nullptr),
    InFunc(false) {
  GMsgs = M;
  InitBinopPrecedence();
  LabelIncr = 0;
  InitIntrinsics();
}

SCParser::~SCParser(){
  TheModule.reset();
  TheFPM.reset();
  NamedValues.clear();
}

void SCParser::InitIntrinsics(){
  Intrins.push_back(static_cast<SCIntrin *>(new SCMax()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCMin()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCLoad()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCStore()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCNot()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCReverse()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCPopcount()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCClz()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCCtz()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCSext()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCZext()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCRotateL()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCRotateR()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCMaj()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCDoz()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCCompress()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCCompressM()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCInsertS()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCInsertZ()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCExtractS()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCExtractZ()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCMerge()));
}

void SCParser::InitPassMap(){
  EPasses.insert(std::pair<std::string,bool>("PromoteMemoryToRegisterPass",true));
  EPasses.insert(std::pair<std::string,bool>("InstructionCombiningPass",true));
  EPasses.insert(std::pair<std::string,bool>("ReassociatePass",true));
  EPasses.insert(std::pair<std::string,bool>("GVNPass",true));
  EPasses.insert(std::pair<std::string,bool>("CFGSimplificationPass",true));
  EPasses.insert(std::pair<std::string,bool>("ConstantPropagationPass",true));
  EPasses.insert(std::pair<std::string,bool>("IndVarSimplifyPass",true));
  EPasses.insert(std::pair<std::string,bool>("LICMPass",true));
  EPasses.insert(std::pair<std::string,bool>("LoopDeletionPass",true));
  EPasses.insert(std::pair<std::string,bool>("LoopIdiomPass",true));
  EPasses.insert(std::pair<std::string,bool>("LoopRerollPass",true));
  EPasses.insert(std::pair<std::string,bool>("LoopRotatePass",true));
  EPasses.insert(std::pair<std::string,bool>("LoopUnswitchPass",true));
}

bool SCParser::SetInputs( std::string B, std::string F ){
  InBuf = B;
  FileName = F;
  if( Lex ){
    delete Lex;
  }
  Lex = new SCLexer();
  TheModule = llvm::make_unique<Module>(StringRef(FileName), TheContext);

  return true;
}

//===----------------------------------------------------------------------===//
// Optimizer
//===----------------------------------------------------------------------===//

std::vector<std::string> SCParser::GetPassList(){
  std::vector<std::string> P;

  for( auto it = EPasses.begin(); it != EPasses.end(); ++it ){
    P.push_back(it->first);
  }

  return P;
}

// Utilizes Levenshtein Edit Distance algorithm from:
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
// Based upon the original common Lisp implementation whereby only two of the columns
// in the matrix are utilized
unsigned SCParser::EditDistance( std::string &s1, std::string &s2 ){
  const std::size_t len1 = s1.size(), len2 = s2.size();
  std::vector<unsigned int> col(len2+1), prevCol(len2+1);

  for (unsigned int i = 0; i < prevCol.size(); i++){
    prevCol[i] = i;
  }

  for (unsigned int i = 0; i < len1; i++) {
    col[0] = i+1;
    for (unsigned int j = 0; j < len2; j++){
      // note that std::min({arg1, arg2, arg3}) works only in C++11,
      // for C++98 use std::min(std::min(arg1, arg2), arg3)
      col[j+1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i]==s2[j] ? 0 : 1) });
    }
    col.swap(prevCol);
  }

  return prevCol[len2];
}

std::string SCParser::GetNearbyString(std::string &Input){
  // build a vector of the known pass names
  std::vector<std::string> Passes;
  for( auto it = EPasses.begin(); it != EPasses.end(); ++it ){
    Passes.push_back(it->first);
  }

  // search the vector
  std::string RtnStr;
  unsigned LowVal;

  // walk all the pass values and find their edit distance from the input
  LowVal = EditDistance(Input,Passes[0]);
  RtnStr = Passes[0];
  for( unsigned i=1; i<Passes.size(); i++ ){
    unsigned tmp = EditDistance(Input,Passes[i]);
    if( tmp < LowVal ){
      LowVal = tmp;
      RtnStr = Passes[i];
    }
  }

  return RtnStr;
}

bool SCParser::CheckPassNames(std::vector<std::string> P){

  bool rtn = true;

  for( unsigned i=0; i<P.size(); i++ ){
    auto Search = EPasses.find(P[i]);
    if( Search == EPasses.end() ){
      // misspelled pass name
      Msgs->PrintMsg( L_ERROR, "Unknown pass name: \"" + P[i]
                                + "\". Did you mean "
                                + GetNearbyString(P[i]) + "?");
      rtn = false;
    }
  }
  return rtn;
}

bool SCParser::DisablePasses(std::vector<std::string> P){
  if( !CheckPassNames(P) ){
    // something is misspelled
    return false;
  }
  // enable everything, just to be pedantic
  for( auto it=EPasses.begin(); it != EPasses.end(); ++it){
    it->second = true;
  }

  // disable individual passes
  for( unsigned i=0; i<P.size(); i++ ){
    for( auto it=EPasses.begin(); it != EPasses.end(); ++it){
      if( it->first == P[i] ){
        it->second = false;
      }
    }
  }
  return true;
}

bool SCParser::EnablePasses(std::vector<std::string> P){
  if( !CheckPassNames(P) ){
    // something is misspelled
    return false;
  }
  // disbale everything, just to be pedantic
  for( auto it=EPasses.begin(); it != EPasses.end(); ++it){
    it->second = false;
  }

  // disable individual passes
  for( unsigned i=0; i<P.size(); i++ ){
    for( auto it=EPasses.begin(); it != EPasses.end(); ++it){
      if( it->first == P[i] ){
        it->second = true;
      }
    }
  }
  return true;
}

bool SCParser::IsPassEnabled(std::string P){
  if( P.length() == 0 ){
    return false;
  }

  std::map<std::string,bool>::iterator it;

  for( it = EPasses.begin(); it != EPasses.end(); ++it ){
    if( it->first == P ){
      return it->second;
    }
  }

  return false;
}

void SCParser::InitModuleandPassManager(){
  // create a new pass manager
  TheFPM = llvm::make_unique<legacy::FunctionPassManager>(TheModule.get());

  // promote allocas to registers
  if( IsPassEnabled("PromoteMemoryToRegisterPass") ){
    TheFPM->add(createPromoteMemoryToRegisterPass());
  }

  // enable simple peephole opts and bit-twiddling opts
  if( IsPassEnabled("InstructionCombiningPass") ){
    TheFPM->add(createInstructionCombiningPass());
  }

  // enable reassociation of epxressions
  if( IsPassEnabled("ReassociatePass") ){
    TheFPM->add(createReassociatePass());
  }

  // eliminate common subexpressions
  if( IsPassEnabled("GVNPass" ) ){
    TheFPM->add(createGVNPass());
  }

  // simplify the control flow graph
  if( IsPassEnabled("CFGSimplificationPass") ){
    TheFPM->add(createCFGSimplificationPass());
  }

  // constant propogations
  if( IsPassEnabled("ConstantPropagationPass") ){
    TheFPM->add(createConstantPropagationPass());
  }

  // induction variable simplification pass
  if( IsPassEnabled("IndVarSimplifyPass") ){
    TheFPM->add(createIndVarSimplifyPass());
  }

  // loop invariant code motion
  if( IsPassEnabled("LICMPass") ){
    TheFPM->add(createLICMPass());
  }

  // loop deletion
  if( IsPassEnabled( "LoopDeletionPass") ){
    TheFPM->add(createLoopDeletionPass());
  }

  // loop idiom
  if( IsPassEnabled( "LoopIdiomPass") ){
    TheFPM->add(createLoopIdiomPass());
  }

  // loop re-roller
  if( IsPassEnabled( "LoopRerollPass") ){
    TheFPM->add(createLoopRerollPass());
  }

  // loop rotation
  if( IsPassEnabled( "LoopRotatePass" ) ){
    TheFPM->add(createLoopRotatePass());
  }

  // loop unswitching
  if( IsPassEnabled( "LoopUnswitchPass" ) ){
    TheFPM->add(createLoopUnswitchPass());
  }

  // Init it!
  TheFPM->doInitialization();
}

bool SCParser::Optimize(){
  InitModuleandPassManager();
  SCParser::IsOpt = true;
  return true;
}

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

bool SCParser::CheckIntrinName( std::string Name ){
  if( Name.length() == 0 ){
    return false;
  }

  for( unsigned i=0; i<Intrins.size(); i++ ){
    if( Name == Intrins[i]->GetKeyword() ){
      return true;
    }
  }

  return false;
}

unsigned SCParser::GetNumIntrinArgs( std::string Name ){
  if( Name.length() == 0 ){
    return 0;
  }

  for( unsigned i=0; i<Intrins.size(); i++ ){
    if( Name == Intrins[i]->GetKeyword() ){
      return Intrins[i]->GetNumInputs();
    }
  }

  return 0;
}

bool SCParser::InsertExternIntrin(){
  for( unsigned i=0; i<Intrins.size(); i++ ){
    // insert the intrinsic definition
    std::string TmpStr = "extern " + Intrins[i]->GetKeyword() + "(";
    for( unsigned j=0; j<Intrins[i]->GetNumInputs(); j++ ){
      TmpStr += "a";
      TmpStr += std::to_string(j);
      if( j != (Intrins[i]->GetNumInputs()-1) ){
        TmpStr += " ";
      }
    }
    TmpStr += ");\n";
    InBuf.insert(0,TmpStr);
  }
  return true;
}

void SCParser::InitBinopPrecedence(){
  BinopPrecedence['='] = 10;
  BinopPrecedence['<'] = 11;
  BinopPrecedence['>'] = 11;
  BinopPrecedence['|'] = 15;
  BinopPrecedence['^'] = 16;
  BinopPrecedence['&'] = 17;
  //BinopPrecedence['!'] = 18;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;
  BinopPrecedence['/'] = 40;
  BinopPrecedence['%'] = 40;

  // dyadic operators
  BinopPrecedence[dyad_shfl]    = 13;
  BinopPrecedence[dyad_shfr]    = 13;
  BinopPrecedence[dyad_shfr]    = 13;
  BinopPrecedence[dyad_eqeq]    = 12;
  BinopPrecedence[dyad_noteq]   = 12;
  BinopPrecedence[dyad_logand]  = 11;
  BinopPrecedence[dyad_logor]   = 11;

}

bool SCParser::Parse(){
  if( InBuf.length() == 0 ){
    Msgs->PrintMsg( L_ERROR, "No StoneCutter input defined");
    return false;
  }

  // insert all the necessary intrinsic externs
  if( !InsertExternIntrin() ){
    Msgs->PrintMsg( L_ERROR,
                    "Could not adjust input buffer to include intrinsic definitions" );
    return false;
  }

#if 0
  std::cout << "---------------------------- DEBUG" << std::endl;
  std::cout << InBuf << std::endl;
  std::cout << "---------------------------- DEBUG" << std::endl;
#endif

  // set the input buffer for the lexer
  if( !Lex ){
    Msgs->PrintMsg( L_ERROR,
                    "The StoneCutter lexer is not initialized" );
    return false;
  }else if( !Lex->SetInput(InBuf) ){
    Msgs->PrintMsg( L_ERROR,
                    "Failed to initialize the input buffer for the lexer" );
    return false;
  }

  // pull the first token
  GetNextToken();

  while(true){
    switch(CurTok){
    case tok_eof:
      if( InFunc ){
        // no close bracket found
        return false;
      }
      return Rtn;
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

  return Rtn;
}

bool SCParser::GetVarAttr( std::string Str, VarAttrs &V ){

  // parse the variable attribute string
  // and determine what type of variable it is

  // common types for speed
  int Idx = 0;
  while( VarAttrEntryTable[Idx].Name != "." ){
    if( Str == VarAttrEntryTable[Idx].Name ){
      V.width     = VarAttrEntryTable[Idx].width;
      V.elems     = VarAttrEntryTable[Idx].elems;
      V.defSign   = VarAttrEntryTable[Idx].IsDefSign;
      V.defVector = VarAttrEntryTable[Idx].IsDefVector;
      V.defFloat  = VarAttrEntryTable[Idx].IsDefFloat;
      V.defRegClass = false;
      return true;
    }
    Idx++;
  }

  if( Str[0] == 'u' ){
    // unsigned variable
    V.elems     = 1;
    V.defSign   = false;
    V.defVector = false;
    V.defFloat  = false;
    V.defRegClass = false;
    V.width     = std::stoi( Str.substr(1,Str.length()-1) );
    return true;
  }else if( Str[0] == 's' ){
    // signed variable
    V.elems = 1;
    V.defSign   = true;
    V.defVector = false;
    V.defFloat  = false;
    V.defRegClass = false;
    V.width     = std::stoi( Str.substr(1,Str.length()-1) );
    return true;
  }

  return false;
}

int SCParser::GetNextToken(){
  CurTok = Lex->GetTok();
#if 0
  std::cout << "CurTok = " << CurTok << std::endl;
#endif
  return CurTok;
}

int SCParser::GetTokPrecedence(){
  if( CurTok == tok_dyad ){
    return BinopPrecedence[StrToDyad()];
  }

  if (!isascii(CurTok))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

int SCParser::StrToDyad(){
  std::string TStr = Lex->GetIdentifierStr();
  if( TStr == "<<" ){
    return dyad_shfl;
  }else if( TStr == ">>" ){
    return dyad_shfr;
  }else if( TStr == "==" ){
    return dyad_eqeq;
  }else if( TStr == "!=" ){
    return dyad_noteq;
  }else if( TStr == "&&" ){
    return dyad_logand;
  }else if( TStr == "||" ){
    return dyad_logor;
  }else if( TStr == ">=" ){
    return dyad_gte;
  }else if( TStr == "<=" ){
    return dyad_lte;
  }else{
    return '.'; // this will induce an error
  }
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

  if( CurTok == ';' ){
    GetNextToken(); // eat ';'
  }

  // Check to see if the call is an intrinsic
  bool Intrin = false;
  if( CheckIntrinName(IdName) ){
    // we have an intrinsic, check the argument list
    Intrin = true;

    if( GetNumIntrinArgs(IdName) != Args.size() ){
      // argument mismatch
      return LogError( IdName + " intrinsic requires " +
                       std::to_string(GetNumIntrinArgs(IdName)) +
                       " arguments." );
    }
  }

  return llvm::make_unique<CallExprAST>(IdName, std::move(Args), Intrin);
}

std::unique_ptr<ExprAST> SCParser::ParseIfExpr() {
  GetNextToken();  // eat the if.

  if( CurTok != '(' )
    return LogError("expected '(' for conditional statement");

  GetNextToken(); // eat the '('

  // condition.
  auto Cond = ParseExpression();
  if (!Cond){
    return nullptr;
  }

  if( CurTok != ')' )
    return LogError("expected ')' for conditional statement");

  GetNextToken(); // eat the ')'

  if( CurTok != '{' )
    return LogError("expected '{' for conditional expression body");
  GetNextToken(); // eat the '{'

  auto Then = ParseExpression();
  if (!Then){
    return nullptr;
  }

  if( CurTok != '}' )
    return LogError("expected '}' for conditional expression body");
  GetNextToken(); // eat the '}'

  // parse the optional else block
  std::unique_ptr<ExprAST> Else = nullptr;
  if( CurTok == tok_else ){
    // parse else{ <Expression> }
    GetNextToken(); // eat the else
    if( CurTok != '{' ){
      LogError("expected '{' for conditional else statement");
    }
    GetNextToken(); // eat the '{'
    Else = ParseExpression();
    if( !Else ){
      return nullptr;
    }
    if( CurTok != '}' ){
      LogError("expected '}' for conditional else statement");
    }
    GetNextToken(); // eat the '}'
  }

  return llvm::make_unique<IfExprAST>(std::move(Cond), std::move(Then),
                                      std::move(Else));
}

std::unique_ptr<ExprAST> SCParser::ParsePrimary() {
  switch (CurTok) {
  default:
    return LogError("unknown token when expecting an expression" );
  case tok_identifier:
    return ParseIdentifierExpr();
  case tok_number:
    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  case tok_if:
    return ParseIfExpr();
  case tok_for:
    return ParseForExpr();
  case tok_var:
    return ParseVarExpr();
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

    // handle dyadic binary operators
    if( BinOp == tok_dyad ){
      // convert BinOp to dyadic operator
      BinOp = StrToDyad();
    }

    GetNextToken(); // eat binop; must be done after dyadic operator conversion

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

// parse the variable definition lists
// this can be one of the following permutations:
// - DATATYPE VARNAME
// - DATATYPE VARNAME = INITIALIZER
// - DATATYPE VARNAME, VARNAME, VARNAME, VARNAME
// - DATATYPE VARNAME = INITIALIZER, VARNAME, VARNAME
std::unique_ptr<ExprAST> SCParser::ParseVarExpr(){
  GetNextToken(); // eat the datatype

  std::vector<std::pair<std::string,std::unique_ptr<ExprAST>>> VarNames;
  std::vector<VarAttrs> Attrs;  // variable attributes

  if( CurTok != tok_identifier )
    return LogError("expected identifier after variable datatype");

  while(true){
    std::string Name = Lex->GetIdentifierStr();
    GetNextToken(); // eat identifier.

    // Read the optional initializer.
    std::unique_ptr<ExprAST> Init = nullptr;
    if (CurTok == '=') {
      GetNextToken(); // eat the '='.

      Init = ParseExpression();
      if (!Init)
        return nullptr;
    }

    // add the new variable to our vector
    VarNames.push_back(std::make_pair(Name, std::move(Init)));
    Attrs.push_back(std::move(Lex->GetVarAttrs()));

    // check for the end of the variable list
    if( CurTok != ',' ){
      break;
    }

    GetNextToken(); // eat the ','

    if( CurTok != tok_identifier )
      LogError("expected identifier list in variable definition");
  }// end parsing the variable list

  auto Body = ParseExpression();
  if (!Body)
    return nullptr;

  return llvm::make_unique<VarExprAST>(std::move(VarNames),
                                       std::move(Attrs),
                                       std::move(Body) );
}

std::unique_ptr<ExprAST> SCParser::ParseForExpr() {
  GetNextToken();  // eat the for.

  if (CurTok != '(' )
    return LogError("expected '(' for loop control statement");

  GetNextToken(); // eat the '('

  if (CurTok != tok_identifier)
    return LogError("expected identifier after for");

  std::string IdName = Lex->GetIdentifierStr();
  GetNextToken();  // eat identifier.

  if (CurTok != '=')
    return LogError("expected '=' after for");
  GetNextToken();  // eat '='.


  auto Start = ParseExpression();
  if (!Start)
    return nullptr;
  if (CurTok != ';')
    return LogError("expected ',' after for start value");
  GetNextToken();

  auto End = ParseExpression();
  if (!End)
    return nullptr;

  // The step value is optional.
  std::unique_ptr<ExprAST> Step;
  if (CurTok == ';') {
    GetNextToken();
    Step = ParseExpression();
    if (!Step)
      return nullptr;
  }

  if (CurTok != ')')
    return LogError("expected ')' for loop control statement");

  GetNextToken(); // eat ')'

  // check for open bracket
  if (CurTok != '{'){
    return LogError("expected '}' for loop control body");
  }
  GetNextToken(); // eat '{'

  std::vector<std::unique_ptr<ExprAST>> BodyExpr;
  while( CurTok != '}' ){
    auto Body = ParseExpression();
    if( !Body )
      return nullptr;
    BodyExpr.push_back(std::move(Body));
  }

  // handle close bracket
  if( CurTok != '}' ){
    return LogError("expected '}' for loop control body");
  }
  GetNextToken(); // eat the '}'

  return llvm::make_unique<ForExprAST>(IdName, std::move(Start),
                                       std::move(End), std::move(Step),
                                       std::move(BodyExpr));
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

VarAttrs SCParser::GetMaxVarAttr(std::vector<VarAttrs> ArgAttrs){
  VarAttrs Attr;
  Attr.elems = 1;
  Attr.defSign = false;
  Attr.defVector = false;
  Attr.defFloat = false;

  unsigned Max = 0;
  for( unsigned i=0; i<ArgAttrs.size(); i++ ){
    if( ArgAttrs[i].width > Max ){
      Max = ArgAttrs[i].width;
    }
  }

  Attr.width = Max;

  return Attr;
}

std::unique_ptr<RegClassAST> SCParser::ParseRegClassDef(){
  if (CurTok != tok_identifier)
    return LogErrorR("Expected regclass name in prototype");

  std::string RName = Lex->GetIdentifierStr();
  GetNextToken();

  if (CurTok != '(')
    return LogErrorR("Expected '(' in regclass prototype");

  std::vector<std::string> ArgNames;          // vector of register names
  std::vector<VarAttrs> ArgAttrs;             // vector of register attributes
  std::vector<std::tuple<std::string,
                         std::string,
                         VarAttrs>> SubRegs;  // vector of subregister tuples

  // try to pull the next identifier
  GetNextToken();

  while (CurTok == tok_var){
    std::string Type = Lex->GetIdentifierStr();
    VarAttrs VAttr;
    if( !GetVarAttr( Type, VAttr ) ){
      return LogErrorR("Unknown variable type: " + Type );
    }

    if( GetNextToken() != tok_identifier ){
      return LogErrorR("Expected register name");
    }

    std::string RegName = Lex->GetIdentifierStr();

    // add them to our vector
    ArgAttrs.push_back(VAttr);
    ArgNames.push_back(RegName);

    // Look for the comma or subregister
    GetNextToken();
    if( CurTok == ',' ){
      // eat the comma
      GetNextToken();
    }else if( CurTok == '(' ){
      // attempt to parse the sub registers

      // eat the (
      GetNextToken();

      // walk all the subregisters
      while( CurTok == tok_var ){
        std::string SType = Lex->GetIdentifierStr();
        VarAttrs SAttr;
        if( !GetVarAttr( SType, SAttr ) ){
          return LogErrorR("Unknown subregister type: " + SType );
        }

        if( GetNextToken() != tok_identifier ){
          return LogErrorR("Expected subregister name" );
        }

        std::string SubReg = Lex->GetIdentifierStr();

        // insert into the vector
        SubRegs.push_back(std::tuple<std::string,std::string,VarAttrs>
                         (RegName,SubReg,SAttr));

        // eat the identifier
        GetNextToken();

        if( CurTok == ')' ){
          // eat the )
          GetNextToken();
          break;
        }else if( CurTok == ',' ){
          // eat the comma but continue reading sub registers
          GetNextToken();
        }else{
          // flag an error
          return LogErrorR("Expected ',' or ')' in subregister list");
        }
      }

      // handle the next comma or ending paren
      if( CurTok == ',' ){
        // eat the comma
        GetNextToken();
      }else{
        break;
      }
    }else{
      break;
    }
  }

  if (CurTok != ')')
    return LogErrorR("Expected ')' in regclass prototype");

  // success.
  GetNextToken(); // eat ')'.

  // push back the register class name as a unique global
  // this will allow us to use this variable in the function prototype
  VarAttrs RVAttr = GetMaxVarAttr(ArgAttrs);
  RVAttr.defRegClass = true;
  ArgAttrs.push_back(RVAttr);
  ArgNames.push_back(RName);

  return llvm::make_unique<RegClassAST>(RName,
                                        std::move(ArgNames),
                                        std::move(ArgAttrs),
                                        std::move(SubRegs));
}

std::unique_ptr<FunctionAST> SCParser::ParseDefinition() {
  GetNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  // check the prototype name against the list of existing intrinics
  if( CheckIntrinName(Proto->getName()) ){
    return LogErrorF("Instruction definition collides with existing StoneCutter intrinsic: " +
              Proto->getName() );
  }

  // check for open instruction body
  if( CurTok != '{' )
    return LogErrorF("Expected '{' to open instruction body");
  if( InFunc ){
    return LogErrorF("Found mismatch '{'");
  }
  GetNextToken(); // eat the '{'
  InFunc = true;

  std::vector<std::unique_ptr<ExprAST>> Exprs;
  while( CurTok != '}' ){
    auto Body = ParseExpression();
    if( !Body )
      return nullptr;
    Exprs.push_back(std::move(Body));
  }

  if( CurTok != '}' )
    return LogErrorF("Expected '}' to close instruction body");

  if( Exprs.size() == 0 ){
    return LogErrorF("Empty instruction definition: " + Proto->getName() );
  }

  GetNextToken(); // eat '}'
  InFunc = false;
  return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(Exprs));
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
    std::vector<std::unique_ptr<ExprAST>> Exprs;
    Exprs.push_back(std::move(E));
    //return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(Exprs));
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
    Rtn = false;
    GetNextToken();
  }
}

void SCParser::HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      //fprintf(stderr, "Read extern: ");
      //FnIR->print(errs());
      //fprintf(stderr, "\n");
      SCParser::FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
    }
  } else {
    // Skip token for error recovery.
    Rtn = false;
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
    Rtn = false;
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
    Rtn = false;
    GetNextToken();
  }
}

void SCParser::HandleFuncClose(){
  if(!ParseCloseBracket()){
    Rtn = false;
    GetNextToken();
  }
}

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//
static unsigned GetLocalLabel(){
  unsigned LocalLabel = SCParser::LabelIncr;
  SCParser::LabelIncr++;
  return LocalLabel;
}

static AllocaInst *CreateEntryBlockAllocaAttr(Function *TheFunction,
                                              const std::string &VarName,
                                              VarAttrs Attr) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                 TheFunction->getEntryBlock().begin());

  if( Attr.defFloat ){
    return TmpB.CreateAlloca(Type::getDoubleTy(SCParser::TheContext), 0,
                             VarName.c_str());
  }else{
    return TmpB.CreateAlloca(Type::getIntNTy(SCParser::TheContext,Attr.width), 0,
                             VarName.c_str());
  }
}

static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          const std::string &VarName) {
  // if variable attributes are undefined, define a dummy u64 value
  VarAttrs A;
  A.width = 64;
  A.elems = 1;
  A.defSign = false;
  A.defVector = false;
  A.defFloat = false;
  return CreateEntryBlockAllocaAttr(TheFunction,VarName,A);
}

Function *getFunction(std::string Name ){
  // see if the function has been added to the current module
  if( auto *F = SCParser::TheModule->getFunction(Name) )
    return F;

  // if not, check whether we can emit the declaration
  auto FI = SCParser::FunctionProtos.find(Name);
  if( FI != SCParser::FunctionProtos.end() ){
    return FI->second->codegen();
  }

  return nullptr;
}

Value *NumberExprAST::codegen() {
  return ConstantInt::get(SCParser::TheContext, APInt(64,Val,false));
}

Value *ForExprAST::codegen() {

  // Create an alloca for the variable in the entry block
  Function *TheFunction = SCParser::Builder.GetInsertBlock()->getParent();
  AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, VarName);

  // Emit the start code first, without 'variable' in scope.
  Value *StartVal = Start->codegen();
  if (!StartVal)
    return nullptr;

  // Store the value into an alloca
  SCParser::Builder.CreateStore(StartVal,Alloca);

  unsigned LocalLabel = GetLocalLabel();

  // Make the new basic block for the loop header, inserting after current
  // block.
  BasicBlock *LoopBB = BasicBlock::Create(SCParser::TheContext,
                                          "loop."+std::to_string(LocalLabel),
                                          TheFunction);

  // Insert an explicit fall through from the current block to the LoopBB.
  Builder.CreateBr(LoopBB);

  // Start insertion in LoopBB.
  Builder.SetInsertPoint(LoopBB);

  // Within the loop, the variable is defined equal to the PHI node.  If it
  // shadows an existing variable, we have to restore it, so save it now.
  AllocaInst *OldVal   = NamedValues[VarName];
  NamedValues[VarName] = Alloca;

  // Emit the body of the loop.  This, like any other expr, can change the
  // current BB.  Note that we ignore the value computed by the body, but don't
  // allow an error.
  if( BodyExpr.size() == 0 ) return nullptr;
  for( unsigned i=0; i<BodyExpr.size(); i++ ){
    BodyExpr[i]->codegen();
  }

  // Emit the step value.
  Value *StepVal = nullptr;
  if (Step) {
    StepVal = Step->codegen();
    if (!StepVal)
      return nullptr;
  } else {
    // If not specified, use u64(0)
    //StepVal = ConstantFP::get(SCParser::TheContext, APFloat(1.0));
    StepVal = ConstantInt::get(SCParser::TheContext, APInt(64,0,false));
  }

  // Compute the end condition.
  Value *EndCond = End->codegen();
  if (!EndCond)
    return nullptr;

  Value *CurVar  = Builder.CreateLoad(Alloca,VarName.c_str());
  //Value *NextVar = Builder.CreateFAdd(CurVar,
  Value *NextVar = Builder.CreateAdd(CurVar,
                                      StepVal,
                                      "nextvar" + std::to_string(LocalLabel));
  Builder.CreateStore(NextVar,Alloca);

  // Convert condition to a bool by comparing non-equal to 0.0.
  //EndCond = Builder.CreateFCmpONE(
  EndCond = Builder.CreateICmpNE(
      //EndCond, ConstantFP::get(SCParser::TheContext,
      //                         APFloat(0.0)),
      //                         "loopcond" + std::to_string(LocalLabel));
      EndCond, ConstantInt::get(SCParser::TheContext,
                               APInt(1,0,false)),
                               "loopcond" + std::to_string(LocalLabel));

  // Create the "after loop" block and insert it.
  BasicBlock *AfterBB =
      BasicBlock::Create(SCParser::TheContext,
                         "afterloop." + std::to_string(LocalLabel),
                         TheFunction);

  // Insert the conditional branch into the end of LoopEndBB.
  Builder.CreateCondBr(EndCond, LoopBB, AfterBB);

  // Any new code will be inserted in AfterBB.
  Builder.SetInsertPoint(AfterBB);

  // Add a new entry to the PHI node for the backedge.

  // Restore the unshadowed variable.
  if (OldVal)
    NamedValues[VarName] = OldVal;
  else
    NamedValues.erase(VarName);

  // for expr always returns 0.0.
  return Constant::getNullValue(Type::getIntNTy(SCParser::TheContext,64));
}

Value *VariableExprAST::codegen() {
  // Look this variable up in the function.
  Value *V = SCParser::NamedValues[Name];
  if (!V){
    // variable wasn't in the function scope, check the globals
    V = SCParser::GlobalNamedValues[Name];
    if( !V ){
      return LogErrorV("Unknown variable name: " + Name );
    }
  }

  return SCParser::Builder.CreateLoad(V,Name.c_str());
}

Value *VarExprAST::codegen() {
  std::vector<AllocaInst *> OldBindings;

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  // walk all the variables and their initializers
  for (unsigned i = 0, e = VarNames.size(); i != e; ++i) {
    const std::string &VarName = VarNames[i].first;
    ExprAST *Init = VarNames[i].second.get();

    // Emit the initializer before adding the variable to scope, this prevents
    // the initializer from referencing the variable itself, and permits
    // nested sets of variable definitions
    Value *InitVal;
    if (Init) {
      InitVal = Init->codegen();
      if (!InitVal)
        return nullptr;
    } else {
      // If not specified, use DATATYPE(0)
      // derive the respective type and init the correct
      // type with the value
      if( Attrs[i].defFloat ){
        InitVal = ConstantFP::get(SCParser::TheContext,
                                  APFloat(0.0));
      }else{
        InitVal = ConstantInt::get(SCParser::TheContext,
                                   APInt(Attrs[i].width,
                                         0,
                                         Attrs[i].defSign));
      }
    }

    AllocaInst *Alloca = CreateEntryBlockAllocaAttr(TheFunction,
                                                    VarName,
                                                    Attrs[i]);
    Builder.CreateStore(InitVal, Alloca);

    OldBindings.push_back(SCParser::NamedValues[VarName]);
    SCParser::NamedValues[VarName] = Alloca;
  }

  // Codegen the body, now that all vars are in scope.
  Value *BodyVal = Body->codegen();
  if (!BodyVal)
    return nullptr;

  // Pop all our variables from scope.
  for (unsigned i = 0, e = VarNames.size(); i != e; ++i)
    SCParser::NamedValues[VarNames[i].first] = OldBindings[i];

  // Return the body computation.
  return BodyVal;
}

Value *BinaryExprAST::codegen() {

  // handle the special case of an assignment operator
  if( Op == '='){
    // Assignment requires the LHS to be an identifier.
    // This assume we're building without RTTI because LLVM builds that way by
    // default.  If you build LLVM with RTTI this can be changed to a
    // dynamic_cast for automatic error checking.
    VariableExprAST *LHSE = static_cast<VariableExprAST *>(LHS.get());
    if (!LHSE)
      return LogErrorV("destination of '=' must be a variable");
    // Codegen the RHS.
    Value *Val = RHS->codegen();
    if (!Val)
      return nullptr;

    // Look up the name in the local and global symbol tables
    Value *Variable = SCParser::NamedValues[LHSE->getName()];
    if (!Variable){
      Variable = SCParser::GlobalNamedValues[LHSE->getName()];
      if( !Variable ){
        return LogErrorV("Unknown variable name: " + LHSE->getName());
      }
    }

    Builder.CreateStore(Val, Variable);
    return Val;
  }

  // handle the remainder of the binary operators
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R){
    return nullptr;
  }

  switch (Op) {
  case '+':
    //return SCParser::Builder.CreateFAdd(L, R, "addtmp");
    return SCParser::Builder.CreateAdd(L, R, "addtmp");
  case '-':
    //return SCParser::Builder.CreateFSub(L, R, "subtmp");
    return SCParser::Builder.CreateSub(L, R, "subtmp");
  case '*':
    //return SCParser::Builder.CreateFMul(L, R, "multmp");
    return SCParser::Builder.CreateMul(L, R, "multmp");
  case '<':
    //L = SCParser::Builder.CreateFCmpULT(L, R, "cmptmp");
    L = SCParser::Builder.CreateICmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return L;
    //return SCParser::Builder.CreateUIToFP(L, Type::getDoubleTy(SCParser::TheContext), "booltmp");
  case '>':
    //L = SCParser::Builder.CreateFCmpUGT(L, R, "cmptmp");
    L = SCParser::Builder.CreateICmpUGT(L, R, "cmptmp");
    return L;
    // Convert bool 0/1 to double 0.0 or 1.0
    //return SCParser::Builder.CreateUIToFP(L, Type::getDoubleTy(SCParser::TheContext), "booltmp");
  case '%':
    return SCParser::Builder.CreateURem(L, R, "modtmp");
  case '/':
    return SCParser::Builder.CreateUDiv(L, R, "divtmp", true);
  case '&':
    return SCParser::Builder.CreateAnd(L, R, "andtmp" );
  case '|':
    return SCParser::Builder.CreateOr(L, R, "ortmp" );
  case '^':
    return SCParser::Builder.CreateXor(L, R, "xortmp" );
  case dyad_shfl:
    return SCParser::Builder.CreateShl(L, R, "shfltmp", false, false );
  case dyad_shfr:
    return SCParser::Builder.CreateLShr(L, R, "lshfrtmp", false );
  case dyad_eqeq:
    return SCParser::Builder.CreateICmpEQ(L, R, "cmpeq" );
  case dyad_noteq:
    return SCParser::Builder.CreateICmpNE(L, R, "cmpeq" );
  case dyad_logand:
    return SCParser::Builder.CreateAnd(L, R, "andtmp" );
  case dyad_logor:
    return SCParser::Builder.CreateOr(L, R, "ortmp" );
  case dyad_gte:
    L = SCParser::Builder.CreateICmpUGE(L, R, "cmptmp");
    return L;
  case dyad_lte:
    L = SCParser::Builder.CreateICmpULE(L, R, "cmptmp");
    return L;
  default:
    return LogErrorV("invalid binary operator");
  }

  // we don't currently support user-defined binary operators, so just return here
  // see chapter 6 in the llvm frontend tutorial
}

Value *CallExprAST::codegen() {
  // Look up the name in the global module table.
  Function *CalleeF = SCParser::TheModule->getFunction(Callee);
  if (!CalleeF){
    return LogErrorV("Unknown function referenced: " + Callee );
  }

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
  // TODO: This eventually needs to look up the variable names
  // in the register class lists
  // If they are found, use the datatype from the register value
  // Otherwise, default to u64
  //std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(SCParser::TheContext));
  std::vector<Type *> Doubles(Args.size(),
                              Type::getIntNTy(SCParser::TheContext,64));
  FunctionType *FT =
      FunctionType::get(Type::getIntNTy(SCParser::TheContext,64), Doubles, false);
      //FunctionType::get(Type::getDoubleTy(SCParser::TheContext), Doubles, false);

  Function *F =
      Function::Create(FT, Function::ExternalLinkage, Name, SCParser::TheModule.get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

Value *RegClassAST::codegen(){

  // registers
  for( unsigned i=0; i<Args.size(); i++ ){
    Type *VType;
    if( (Attrs[i].defFloat) && (Attrs[i].width==32) ){
      VType = Type::getFloatTy(SCParser::TheContext);
    }else if( (Attrs[i].defFloat) && (Attrs[i].width==64) ){
      VType = Type::getDoubleTy(SCParser::TheContext);
    }else{
      VType = Type::getIntNTy(SCParser::TheContext,Attrs[i].width);
    }

    GlobalVariable *val = new GlobalVariable(*SCParser::TheModule,
                                             VType,
                                             false,
                                             GlobalValue::ExternalLinkage,
                                             nullptr,
                                             Twine(Args[i]),
                                             nullptr,
                                             GlobalVariable::NotThreadLocal,
                                             0 );
    if( !val ){
      return LogErrorV( "Failed to lower register class to global: regclass = "+
                        Name + " register=" + Args[i]);
    }

    GlobalNamedValues[Args[i]] = val;

    if( !Attrs[i].defRegClass ){
      // this is a normal register
      // insert a special attribute to track the register
      val->addAttribute("register",Args[i]);
      // insert a special attribute to track the parent register class
      val->addAttribute("regclass", Name);
    }else{
      // this is a register class
      // add a special attribute token
      val->addAttribute("regfile", Args[i] );
    }
  }

  // subregisters
  std::vector<std::tuple<std::string,std::string,VarAttrs>>::iterator it;
  for( it=SubRegs.begin(); it != SubRegs.end(); ++it ){
    Type *SType;
    if( (std::get<2>(*it).defFloat) && (std::get<2>(*it).width==32) ){
      SType = Type::getFloatTy(SCParser::TheContext);
    }else if( (std::get<2>(*it).defFloat) && (std::get<2>(*it).width==64) ){
      SType = Type::getDoubleTy(SCParser::TheContext);
    }else{
      SType = Type::getIntNTy(SCParser::TheContext,std::get<2>(*it).width);
    }

    GlobalVariable *val = new GlobalVariable(*SCParser::TheModule,
                                             SType,
                                             false,
                                             GlobalValue::ExternalLinkage,
                                             nullptr,
                                             Twine(std::get<1>(*it)),
                                             nullptr,
                                             GlobalVariable::NotThreadLocal,
                                             0 );

    if( !val ){
      return LogErrorV( "Failed to lower subregister to global: subreg = " +
                        std::get<1>(*it) + " register=" + std::get<0>(*it) );
    }

    //GlobalNamedValues[std::get<1>(*it)] = GlobalNamedValues[std::get<0>(*it)];
    GlobalNamedValues[std::get<1>(*it)] = val;

    // insert a special attribute to track the subregister
    val->addAttribute("subregister",std::get<1>(*it));
    // insert a special attribute to track the subregister to register mapping
    val->addAttribute("register",std::get<0>(*it));
  }

  return nullptr;
}

Function *FunctionAST::codegen() {
  // Transfer ownership of the prototype to the FunctionsProtos map
  // keep a reference for use below
  auto &P = *Proto;
  SCParser::FunctionProtos[Proto->getName()] = std::move(Proto);
  Function *TheFunction = getFunction(P.getName());
  if( !TheFunction )
    return nullptr;

  // We don't currently support emitting user-defined binary operators
  // This is Chapter6 in the LLVM Frontend tutorial

  // Create a new basic block to start insertion into
  BasicBlock *BB = BasicBlock::Create(SCParser::TheContext,
                                      "entry."+P.getName(),
                                      TheFunction);
  SCParser::Builder.SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map
  SCParser::NamedValues.clear();
  for( auto &Arg : TheFunction->args()){
    // create an alloca for this variable
    // TODO: we eventually need to record the function arg types
    //       such that we can correctly setup the alloca's
    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction,Arg.getName());

    // store the initial value into the alloca
    Builder.CreateStore(&Arg, Alloca);

    // add arguments to the variable symbol table
    SCParser::NamedValues[Arg.getName()] = Alloca;
  }

  // codegen all the body elements
  // save the last element for the return value
  for( unsigned i=0; i<Body.size()-1; i++ ){
    Value *BVal = Body[i]->codegen();
    if( !BVal )
      return nullptr;
  }

  if( Value *RetVal = Body[Body.size()-1]->codegen() ){
    // Finish off the function
    Builder.CreateRet(RetVal);
    verifyFunction(*TheFunction);
    if( SCParser::IsOpt ){
      SCParser::TheFPM->run(*TheFunction);
    }
    return TheFunction;
  }

  TheFunction->eraseFromParent();
  return nullptr;
}

Value *IfExprAST::codegen() {
  Value *CondV = Cond->codegen();
  if (!CondV){
    return nullptr;
  }

  // Retrieve a unique local label
  unsigned LocalLabel = GetLocalLabel();

  // Convert condition to a bool by comparing non-equal to 0.0.
  //CondV = Builder.CreateFCmpONE(
  //    CondV, ConstantFP::get(SCParser::TheContext, APFloat(0.0)),
  //    "ifcond."+std::to_string(LocalLabel));
  CondV = Builder.CreateICmpNE(
      CondV, ConstantInt::get(SCParser::TheContext, APInt(1,0,false)),
      "ifcond."+std::to_string(LocalLabel));

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  BasicBlock *ThenBB = BasicBlock::Create(SCParser::TheContext,
                                          "then."+std::to_string(LocalLabel),
                                          TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(SCParser::TheContext,
                                          "else."+std::to_string(LocalLabel));
  BasicBlock *MergeBB = BasicBlock::Create(SCParser::TheContext,
                                           "ifcont."+std::to_string(LocalLabel));

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  // Emit then value.
  Builder.SetInsertPoint(ThenBB);

  Value *ThenV = Then->codegen();
  if (!ThenV){
    return nullptr;
  }

  Builder.CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = Builder.GetInsertBlock();

  // Emit else block.
  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);

  Value *ElseV;
  if( Else != nullptr ){
    ElseV = Else->codegen();
    if (!ElseV)
      return nullptr;
  }

  Builder.CreateBr(MergeBB);
  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = Builder.GetInsertBlock();

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  //PHINode *PN = Builder.CreatePHI(Type::getDoubleTy(SCParser::TheContext),
  //                                2, "iftmp."+std::to_string(LocalLabel));
  PHINode *PN = Builder.CreatePHI(Type::getIntNTy(SCParser::TheContext,64),
                                  2, "iftmp."+std::to_string(LocalLabel));

  PN->addIncoming(ThenV, ThenBB);
  if( Else != nullptr ){
    PN->addIncoming(ElseV, ElseBB);
  }
  return PN;
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
  SCParser::GMsgs->PrintMsg( L_ERROR, Str );
  return nullptr;
}

// EOF
