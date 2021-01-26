//
// _SCParser_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
std::map<std::string, unsigned> SCParser::PipeInstances;
unsigned SCParser::LabelIncr;
bool SCParser::IsOpt = false;
bool SCParser::IsPipe = false;
SCMsg *SCParser::GMsgs = nullptr;
MDNode *SCParser::NameMDNode = nullptr;
MDNode *SCParser::InstanceMDNode = nullptr;
MDNode *SCParser::PipelineMDNode = nullptr;

SCParser::SCParser(std::string B, std::string F, SCOpts *O, SCMsg *M)
  : CurTok(-1), InBuf(B), FileName(F), Opts(O), Msgs(M), Lex(new SCLexer()),
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
  InitPassMap();
  InitIntrinsics();
}

SCParser::~SCParser(){
  TheModule.reset();
  TheFPM.reset();
  NamedValues.clear();
  Intrins.clear();
  delete Lex;
  SCParser::NamedValues.clear();
  SCParser::GlobalNamedValues.clear();
  SCParser::FunctionProtos.clear();
  SCParser::PipeInstances.clear();
}

void SCParser::InitIntrinsics(){
  Intrins.push_back(static_cast<SCIntrin *>(new SCMax()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCMin()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCLoad()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCStore()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCLoadElem()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCStoreElem()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCNot()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCNeg()));
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
  Intrins.push_back(static_cast<SCIntrin *>(new SCConcat()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCLss()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCFence()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCBsel()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCNop()));
}

void SCParser::InitPassMap(){
//#if 0
  EPasses.insert(std::pair<std::string,bool>("PromoteMemoryToRegisterPass",true));
  EPasses.insert(std::pair<std::string,bool>("InstructionCombiningPass",true));
  EPasses.insert(std::pair<std::string,bool>("ReassociatePass",true));
//#endif
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
        if( Opts->IsVerbose() )
          Msgs->PrintRawMsg( "Disabling LLVM Pass: " + it->first );
        it->second = false;
      }
    }
  }
  return true;
}

void SCParser::EnableAllPasses(){
  for( auto it=EPasses.begin(); it != EPasses.end(); ++it){
    it->second = true;
  }
}

bool SCParser::EnablePasses(std::vector<std::string> P){
  if( !CheckPassNames(P) ){
    // something is misspelled
    return false;
  }
  // disable everything, just to be pedantic
  for( auto it=EPasses.begin(); it != EPasses.end(); ++it){
    it->second = false;
  }

  // disable individual passes
  for( unsigned i=0; i<P.size(); i++ ){
    for( auto it=EPasses.begin(); it != EPasses.end(); ++it){
      if( it->first == P[i] ){
        if( Opts->IsVerbose() )
          Msgs->PrintRawMsg( "Enabling LLVM Pass: " + it->first );
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
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: PromoteMemoryToRegisterPass");
    TheFPM->add(createPromoteMemoryToRegisterPass());
  }

  // enable simple peephole opts and bit-twiddling opts
  if( IsPassEnabled("InstructionCombiningPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: InstructionCombiningPass");
    TheFPM->add(createInstructionCombiningPass());
  }

  // enable reassociation of epxressions
  if( IsPassEnabled("ReassociatePass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: ReassociatePass");
    TheFPM->add(createReassociatePass());
  }

  // eliminate common subexpressions
  if( IsPassEnabled("GVNPass" ) ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: GVNPass");
    TheFPM->add(createGVNPass());
  }

  // simplify the control flow graph
  if( IsPassEnabled("CFGSimplificationPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: CFGSimplificationPass");
    TheFPM->add(createCFGSimplificationPass());
  }

  // constant propogations
  if( IsPassEnabled("ConstantPropagationPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: ConstantPropagationPass");
    TheFPM->add(createConstantPropagationPass());
  }

  // induction variable simplification pass
  if( IsPassEnabled("IndVarSimplifyPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: IndVarSimplifyPass");
    TheFPM->add(createIndVarSimplifyPass());
  }

  // loop invariant code motion
  if( IsPassEnabled("LICMPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: LICMPass");
    TheFPM->add(createLICMPass());
  }

  // loop deletion
  if( IsPassEnabled( "LoopDeletionPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: LoopDeletionPass");
    TheFPM->add(createLoopDeletionPass());
  }

  // loop idiom
  if( IsPassEnabled( "LoopIdiomPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: LoopIdiomPass");
    TheFPM->add(createLoopIdiomPass());
  }

  // loop re-roller
  if( IsPassEnabled( "LoopRerollPass") ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: LoopRerollPass");
    TheFPM->add(createLoopRerollPass());
  }

  // loop rotation
  if( IsPassEnabled( "LoopRotatePass" ) ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: LoopRotatePass");
    TheFPM->add(createLoopRotatePass());
  }

  // loop unswitching
  if( IsPassEnabled( "LoopUnswitchPass" ) ){
    if( Opts->IsVerbose() )
      Msgs->PrintRawMsg( "Executing LLVM Pass: LoopUnswitchPass");
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

  // reset the character entry point to the beginning
  Lex->Reset();

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
    case tok_pipeline:
      HandlePipeline();
      break;
    case tok_instf:
      HandleInstFormat();
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

bool SCParser::GetFieldAttr( std::string Str, SCInstField &F ){
  // parse the incoming string and determine if it matches
  // an existing instruction format field type
  if( (Str == "enc") || (Str == "ENC") ){
    F = field_enc;
    return true;
  }else if( (Str == "reg") || (Str == "REG") ){
    F = field_reg;
    return true;
  }else if( (Str == "imm") || (Str == "IMM") ){
    F = field_imm;
    return true;
  }

  F = field_unk;
  return false;
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
      V.elems2D   = VarAttrEntryTable[Idx].elems2D;
      V.defSign   = VarAttrEntryTable[Idx].IsDefSign;
      V.defVector = VarAttrEntryTable[Idx].IsDefVector;
      V.defFloat  = VarAttrEntryTable[Idx].IsDefFloat;
      V.defElem   = VarAttrEntryTable[Idx].IsDefElem;
      V.defMatrix = VarAttrEntryTable[Idx].IsDefMatrix;
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

  std::vector<std::unique_ptr<ExprAST>> ThenExpr;
  while( CurTok != '}' ){
    auto Body = ParseExpression();
    if( !Body )
      return nullptr;
    ThenExpr.push_back(std::move(Body));
  }

  if( CurTok != '}' )
    return LogError("expected '}' for conditional expression body");
  GetNextToken(); // eat the '}'

  // parse the optional else block
  std::unique_ptr<ExprAST> Else = nullptr;
  std::vector<std::unique_ptr<ExprAST>> ElseExpr;
  if( CurTok == tok_else ){
    // parse else{ <Expression> }
    GetNextToken(); // eat the else
    if( CurTok != '{' ){
      LogError("expected '{' for conditional else statement");
    }
    GetNextToken(); // eat the '{'

    while( CurTok != '}' ){
      auto Body = ParseExpression();
      if( !Body )
        return nullptr;
      ElseExpr.push_back(std::move(Body));
    }
    if( CurTok != '}' ){
      LogError("expected '}' for conditional else statement");
    }
    GetNextToken(); // eat the '}'
  }

  return llvm::make_unique<IfExprAST>(std::move(Cond),
                                      std::move(ThenExpr),
                                      std::move(ElseExpr));
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
  case tok_while:
    return ParseWhileExpr();
  case tok_do:
    return ParseDoWhileExpr();
  case tok_var:
    return ParseVarExpr();
  case tok_pipe:
    return ParsePipeExpr();
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
    Attrs.push_back(Lex->GetVarAttrs());

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

std::unique_ptr<ExprAST> SCParser::ParseDoWhileExpr(){

  GetNextToken(); // eat the do

  if (CurTok != '{')
    return LogError("expected '{' for do while loop control statement");

  GetNextToken(); // eat the '{'

  std::vector<std::unique_ptr<ExprAST>> BodyExpr;
  while( CurTok != '}' ){
    auto Body = ParseExpression();
    if( !Body )
      return nullptr;
    BodyExpr.push_back(std::move(Body));
  }

  // handle close bracket
  if( CurTok != '}' )
    return LogError("expected '}' for do while loop control body");

  GetNextToken(); // eat the '}'

  if( CurTok != tok_while )
    return LogError("expected 'while' for do while loop control");

  GetNextToken(); // eat the 'do'

  if( CurTok != '(' )
    return LogError("expected '(' after 'do' token in do while loop control");

  GetNextToken(); // eat the '('

  // parse the while conditional expression
  auto Cond = ParseExpression();
  if (!Cond)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')' for do while loop control statement");

  GetNextToken(); // eat the ')'

  return llvm::make_unique<DoWhileExprAST>(std::move(Cond),std::move(BodyExpr));
}

std::unique_ptr<ExprAST> SCParser::ParseWhileExpr(){

  GetNextToken(); // eat the while

  if (CurTok != '(' )
    return LogError("expected '(' for while loop control statement");

  GetNextToken(); // eat the '('

  // parse the while conditional expression
  auto Cond = ParseExpression();
  if (!Cond)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')' for while loop control statement");

  GetNextToken(); // eat ')'

  // check for open bracket
  if (CurTok != '{'){
    return LogError("expected '{' for while loop control body");
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
    return LogError("expected '}' for while loop control body");
  }
  GetNextToken(); // eat the '}'

  return llvm::make_unique<WhileExprAST>(std::move(Cond),std::move(BodyExpr));
}

std::unique_ptr<ExprAST> SCParser::ParsePipeExpr() {
  if( IsPipe )
    return LogError("Nested pipes are not supported");

  GetNextToken(); // eat the 'pipe'

  if (CurTok != tok_identifier)
    return LogError("expected identifier after pipe");

  std::string PipeName = Lex->GetIdentifierStr();
  GetNextToken();  // eat identifier.

  std::string PipeLine;

  if( CurTok == ':' ){
    GetNextToken();
    if( CurTok != tok_identifier)
      return LogError("expected pipeline identifier after 'pipe:'");
    PipeLine = Lex->GetIdentifierStr();
    GetNextToken();

    std::map<std::string, GlobalVariable*>::iterator it;
    it = SCParser::GlobalNamedValues.find(PipeLine);
    if( it == SCParser::GlobalNamedValues.end() ){
      return LogError("Pipeline=" + PipeLine + " was not previously defined");
    }
  }

  if (CurTok != '{' )
    return LogError("expected '{' for while loop control statement");

  GetNextToken(); // eat the '{'

  IsPipe = true;  // mark the fact that we're inside a pipe expression

  // get the instance of the current pipe
  unsigned Instance = 0;
  std::map<std::string,unsigned>::iterator it = PipeInstances.find(PipeName);
  if( it != PipeInstances.end() ){
    // increment the instance count
    it->second++;
    Instance = it->second;
  }else{
    // first instance of the pipe
    PipeInstances.insert(std::make_pair(PipeName,0));
  }

  std::vector<std::unique_ptr<ExprAST>> BodyExpr;
  while( CurTok != '}' ){
    auto Body = ParseExpression();
    if( !Body )
      return nullptr;
    BodyExpr.push_back(std::move(Body));
  }

  if( BodyExpr.size() == 0 ){
    return LogError("Pipeline=" + PipeLine + " has no body");
  }

  // handle close bracket
  if( CurTok != '}' )
    return LogError("expected '}' for do while loop control body");

  GetNextToken(); // eat the '}'
  IsPipe = false;

  return llvm::make_unique<PipeExprAST>(PipeName,PipeLine,Instance,std::move(BodyExpr));
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
  }else{
    return LogError("expected ';' for loop control step statement");
  }

  if (CurTok != ')')
    return LogError("expected ')' for loop control statement");

  GetNextToken(); // eat ')'

  // check for open bracket
  if (CurTok != '{'){
    return LogError("expected '{' for loop control body");
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

  // parse the optional inst format
  std::string InstFormat;
  if(CurTok == ':'){
    GetNextToken();
    if( CurTok != tok_identifier )
      return LogErrorP("Expected identifier in instruction prototype instruction format: 'name:format'");
    InstFormat = Lex->GetIdentifierStr();
    GetNextToken(); // eat the identifier
  }

  if (CurTok != '(')
    return LogErrorP("Expected '(' in prototype");

  std::vector<std::string> ArgNames;
  while (GetNextToken() == tok_identifier){
    std::string LVar = Lex->GetIdentifierStr();
    Value *V = SCParser::GlobalNamedValues[LVar];
    if( !V ){
      // not a global variable
      ArgNames.push_back(LVar);
    }
  }
#if 0
  while (GetNextToken() == tok_identifier)
    ArgNames.push_back(Lex->GetIdentifierStr());
#endif
  if (CurTok != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  GetNextToken(); // eat ')'.

  return llvm::make_unique<PrototypeAST>(FnName, InstFormat, std::move(ArgNames));
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

std::unique_ptr<InstFormatAST> SCParser::ParseInstFormatDef(){

  if(CurTok != tok_identifier)
    return LogErrorIF("Expected instruction format name in prototype");

  std::string FName = Lex->GetIdentifierStr();
  GetNextToken();

  if(CurTok != '(' )
    return LogErrorIF("Expected '(' in instruction format prototype= " + FName);

  std::vector<std::tuple<std::string,
                           SCInstField,
                           std::string,
                           unsigned>> Fields; // vector of field tuples

  // try to pull the next identifier
  GetNextToken();

  // parse all the field names
  while( CurTok == tok_identifier ){
    std::string TypeStr = Lex->GetIdentifierStr();
    SCInstField FieldType = field_unk;
    if( !GetFieldAttr( TypeStr, FieldType) ){
      return LogErrorIF("Unknown field type: " + TypeStr );
    }

    // if the field type is a register, read the register class type
    std::string RegType = ".";
    if( FieldType == field_reg ){
      GetNextToken();
      if( CurTok != '[' )
        return LogErrorIF("Expected '[' in register field definition: " + FName );

      GetNextToken();
      if( CurTok != tok_identifier )
        return LogErrorIF("Expected register class type in register field definition: instruction format=" + FName );
      RegType = Lex->GetIdentifierStr();

      GetNextToken();
      if( CurTok != ']' )
        return LogErrorIF("Expected ']' in register field definition: " + FName );
    }

    // eat the previous token
    GetNextToken();
    if( CurTok != tok_identifier )
      return LogErrorIF("Expected field name in field definition: " + FName );

    std::string FieldName = Lex->GetIdentifierStr();

    // look for the optional width specifier
    unsigned Width = 0;
    GetNextToken();
    if( CurTok == ':' ){
      // eat the colon
      GetNextToken();

      // parse a numeric value
      if( CurTok != tok_number ){
        return LogErrorIF("Expected numeric identifier in field width defintition: " + FName );
      }

      Width = (unsigned)(Lex->GetNumVal());
      GetNextToken();
    }

    // add it to the vector
    Fields.push_back(std::tuple<std::string,SCInstField,std::string,unsigned>
                     (FieldName,FieldType,RegType,Width));

    // look for commas and the end of the definition
    if( CurTok == ',' ){
      // eat the comma
      GetNextToken();
    }
  }

  if (CurTok != ')')
    return LogErrorIF("Expected ')' in instruction format");

  // success
  GetNextToken(); // eat ')'.

  return llvm::make_unique<InstFormatAST>(FName,
                                          std::move(Fields));
}

std::unique_ptr<PipelineAST> SCParser::ParsePipeline(){
  GetNextToken(); //eat pipeline
  return ParsePipelineDef();
}

std::unique_ptr<PipelineAST> SCParser::ParsePipelineDef(){
  if( CurTok != tok_identifier )
    return LogErrorPI("Expected pipeline name in prototype");

  std::string PName = Lex->GetIdentifierStr();
  GetNextToken();

  std::map<std::string, GlobalVariable*>::iterator it;
  it = SCParser::GlobalNamedValues.find(PName);
  if( it != SCParser::GlobalNamedValues.end() ){
    return LogErrorPI("Found duplicate pipeline name: " + PName );
  }


  if (CurTok != '(')
    return LogErrorPI("Expected '(' in pipeline prototype");

  std::vector<std::string> Attrs; // vector of pipeline attributes

  // pull the next identifier
  GetNextToken(); // eat the '('

  while (CurTok == tok_pipeattr){

    Attrs.push_back(Lex->GetIdentifierStr());

    GetNextToken();

    // --
    // Parse a comma, if a comma exist, move to the next attribute
    // --
    if( CurTok == ',' ){
      // eat the comma
      GetNextToken();
    }else{
      break; // probably the end of the attribute list
    }
  }// end while loop

  if (CurTok != ')')
    return LogErrorPI("Expected ')' in pipeline prototype");

  // success.
  GetNextToken(); // eat ')'.

  return llvm::make_unique<PipelineAST>(PName,
                                        std::move(Attrs));
}

std::unique_ptr<RegClassAST> SCParser::ParseRegClassDef(){
  if (CurTok != tok_identifier)
    return LogErrorR("Expected regclass name in prototype");

  std::string RName = Lex->GetIdentifierStr();
  GetNextToken();

  if (CurTok != '(')
    return LogErrorR("Expected '(' in regclass prototype");

  std::string PCName;                         // name of the PC register
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

    // init the register attrs
    VAttr.defReadOnly   = false;
    VAttr.defReadWrite  = true;  // default to true
    VAttr.defCSR        = false;
    VAttr.defAMS        = false;
    VAttr.defTUS        = false;
    VAttr.defShared     = false;
    VAttr.defVector     = false;
    VAttr.defMatrix     = false;
    VAttr.elems         = 0;
    VAttr.elems2D       = 0;
    // VAttr.xIdx          = 0;
    // VAttr.yIdx          = 0;

    // add them to our vector
    ArgAttrs.push_back(VAttr);
    ArgNames.push_back(RegName);

    // Look for the an attribute list, subregister list or comma
    GetNextToken();

    // --
    // Parse a brace, if a brace exists, parse an attribute list
    // --
    if( CurTok == '[' ){
      // attempt to parse the attribute field
      // eat the [
      GetNextToken();

      unsigned L_VAttr = ArgAttrs.size()-1;

      if( CurTok != tok_identifier ){
        return LogErrorR("Expected register attribute in [..]");
      }

      while( CurTok == tok_identifier ){
        std::string LA = Lex->GetIdentifierStr();
        if( LA == "PC" ){
          PCName = RegName;
        }else if( LA == "RO" ){
          ArgAttrs[L_VAttr].defReadOnly = true;
          ArgAttrs[L_VAttr].defReadWrite = false;
        }else if( LA == "RW" ){
          ArgAttrs[L_VAttr].defReadWrite = true;
          ArgAttrs[L_VAttr].defReadOnly = false;
        }else if( LA == "CSR" ){
          ArgAttrs[L_VAttr].defCSR = true;
        }else if( LA == "AMS" ){
          ArgAttrs[L_VAttr].defAMS = true;
        }else if( LA == "TUS" ){
          ArgAttrs[L_VAttr].defTUS = true;
        }else if( (LA == "Shared") || (LA == "SHARED") ){
          ArgAttrs[L_VAttr].defShared = true;
        }else{
          return LogErrorR("Unknown register attribute type: " + LA);
        }

        if( ArgAttrs[L_VAttr].defReadOnly && ArgAttrs[L_VAttr].defReadWrite ){
          return LogErrorR("Cannot set Read-Only and Read-Write attributes on register=" + RegName );
        }

        // eat the identifier
        GetNextToken();

        if( CurTok == ',' ){
          // eat the comma
          GetNextToken();
        }else if( CurTok == ']' ){
          // eat the closing brace
          GetNextToken();
          break;
        }else{
          return LogErrorR("Expected ',' or closing brace ']' in register attribute list for register=" + RegName);
        }
      }
    }
    // --
    // Parse an angle bracket, if an angle bracket exists, parse a vector/matrix
    // --
    if( CurTok == '<' ){
      // attempt to parse the vector
      // eat the <
      GetNextToken();

      unsigned L_VAttr = ArgAttrs.size()-1;

      ArgAttrs[L_VAttr].defVector = true;
      // will store dimensions of vector/mat reg
      // std::vector<unsigned> Dimensions;

      // parse a numeric value
      if( CurTok != tok_number ){
        return LogErrorR("Expected numerical value for dimension "); 
      }
      
      while( CurTok == tok_number ){
        // get dimension value
        unsigned dim = (unsigned)(Lex->GetNumVal());

        if( !ArgAttrs[L_VAttr].defMatrix )
          ArgAttrs[L_VAttr].elems = dim;

        else{ 
          // check if matrix more than 2 dimesions
          if( ArgAttrs[L_VAttr].elems2D )
            return LogErrorR("Matrices greater than 2-Dimensions are not currently supported. Please adjust " + RegName);
          // assign value after comma to ydim 
          ArgAttrs[L_VAttr].elems2D = dim;
        }

        // add them to our vector
        // ArgAttrs.push_back(VAttr);
        // eat the identifier
        GetNextToken();

        if( CurTok == ',' ){
          // must be matrix 
          ArgAttrs[L_VAttr].defVector = false;
          ArgAttrs[L_VAttr].defMatrix = true;
          // eat the comma
          GetNextToken();
        }else if( CurTok == '>' ){
          if( !ArgAttrs[L_VAttr].defMatrix ){
            ArgAttrs[L_VAttr].defVector = true;
            ArgAttrs[L_VAttr].defElem = false;
            // ArgAttrs.push_back(VecVAttr);
            // ArgNames.push_back(RegName);
            for( unsigned i=0; i<ArgAttrs[L_VAttr].elems; i++ ){
            // handle vectors
            // create new name based on containing reg
            std::string VecElemName = RegName + "_" + std::to_string(i);

            // init the reg attrs for vector element
            VarAttrs VecElemVAttr = ArgAttrs[L_VAttr];
            VecElemVAttr.defVector     = true;
            VecElemVAttr.defElem       = true;
            VecElemVAttr.xIdx          = i;
            VecElemVAttr.containers    = RegName;
            
            // add to vector
            ArgAttrs.push_back(VecElemVAttr);
            ArgNames.push_back(VecElemName);
            }
          }
          // Matrix Handling
          else{
            VarAttrs MatVecVAttr = ArgAttrs[L_VAttr]; 
            // MatVecVAttr.defMatrix     = true;
            MatVecVAttr.defVector     = true;
            MatVecVAttr.defElem       = false;
            MatVecVAttr.containers    = RegName;
            for( unsigned i=0; i<ArgAttrs[L_VAttr].elems; i++ ){
              std::string MatColName = RegName + "_col" + std::to_string(i);
              VarAttrs MatColVAttr = MatVecVAttr;
              MatColVAttr.xIdx = i;
              MatColVAttr.yIdx = -1;
              MatColVAttr.elems = ArgAttrs[L_VAttr].elems;
              ArgAttrs.push_back(MatColVAttr);
              ArgNames.push_back(MatColName);
            }
            for( unsigned j=0; j<ArgAttrs[L_VAttr].elems2D; j++ ){
              std::string MatRowName = RegName + "_row" + std::to_string(j);
              VarAttrs MatRowVAttr = MatVecVAttr;
              MatRowVAttr.yIdx = j;
              MatRowVAttr.xIdx = -1;
              MatRowVAttr.elems = ArgAttrs[L_VAttr].elems2D;
              ArgAttrs.push_back(MatRowVAttr);
              ArgNames.push_back(MatRowName);
            }
            if( ArgAttrs[L_VAttr].elems == ArgAttrs[L_VAttr].elems2D) {
              std::string MatDiagName = RegName + "_diag";
              VarAttrs MatDiagVAttr = MatVecVAttr;
              MatDiagVAttr.xIdx = -1;
              MatDiagVAttr.yIdx = -1;
              MatDiagVAttr.elems = ArgAttrs[L_VAttr].elems;
              ArgAttrs.push_back(MatDiagVAttr);
              ArgNames.push_back(MatDiagName);
            }
            for( unsigned i=0; i<ArgAttrs[L_VAttr].elems; i++ ){
              for( unsigned j=0; j<ArgAttrs[L_VAttr].elems2D; j++ ){
                std::string MatElemName = RegName + "_row" + std::to_string(i) + "_col" + std::to_string(j);
                VarAttrs MatElemAttrs = ArgAttrs[L_VAttr];
                MatElemAttrs.defMatrix     = true;
                MatElemAttrs.defElem       = true;
                MatElemAttrs.defVector     = false;
                MatElemAttrs.xIdx          = i;
                MatElemAttrs.yIdx          = j;
                if( (i == j) && (ArgAttrs[L_VAttr].elems == ArgAttrs[L_VAttr].elems2D) ) {
                  MatElemAttrs.containers += RegName + "_diag, " ;
                }
                MatElemAttrs.containers   += (RegName + "_row" + std::to_string(i) + ", ");
                MatElemAttrs.containers   += RegName + "_col" + std::to_string(j) + ", ";
                MatElemAttrs.containers   += RegName;
                ArgAttrs.push_back(MatElemAttrs);
                ArgNames.push_back(MatElemName);
              }
            }
          }
        // eat the closing angle bracket
        GetNextToken();
        break;
      }else{
        return LogErrorR("Expected ',' or closing brace '>' in matrix register dimension list for " + RegName);
        }
      }
    }
    // --
    // Parse an open paren, if an open paren exists, parse a subregister list
    // --
    if( CurTok == '(' ){
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
    }

    // --
    // Parse a comma, if a comma exist, move to the next register
    // --
    if( CurTok == ',' ){
      // eat the comma
      GetNextToken();
    }else{
      break;
    }

  } //while (CurTok == tok_var)

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
                                        PCName,
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

std::unique_ptr<InstFormatAST> SCParser::ParseInstFormat(){
  GetNextToken(); //eat instformat
  return ParseInstFormatDef();
}

std::unique_ptr<FunctionAST> SCParser::ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    // Make an anonymous proto.
    auto Proto = llvm::make_unique<PrototypeAST>("__anon_expr",
                                                 "",
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

void SCParser::HandlePipeline(){
  // evaluate the pipeline definition
  if( auto PipelineAST = ParsePipeline()){
    if( auto *FnIR = PipelineAST->codegen()){
    }
  }else{
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

void SCParser::HandleInstFormat() {
  // evaluate an instruction definition
  if(auto InstFormatAST = ParseInstFormat()){
    if( auto *FnIR = InstFormatAST->codegen()){
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
    AllocaInst *AI = TmpB.CreateAlloca(Type::getDoubleTy(SCParser::TheContext), 0,
                             VarName.c_str());
    if( SCParser::NameMDNode ){
      AI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      AI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      AI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }
    return AI;
  }else{
    AllocaInst *AI = TmpB.CreateAlloca(Type::getIntNTy(SCParser::TheContext,Attr.width), 0,
                             VarName.c_str());
    if( SCParser::NameMDNode ){
      AI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      AI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      AI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }
    return AI;
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

Value *PipeExprAST::codegen() {

  // Emit the body of the pipe stage
  if( BodyExpr.size() == 0 ) return nullptr;

  // Retrieve the function so we can add the necessary attributes
  // Pipeline attribute names are monotonically numbered from 0->N
  // This allows us to have a single attribute with a list of pipeline stages
  // included therein
  Function *TheFunction = Builder.GetInsertBlock()->getParent();
  AttributeSet AttrSet = TheFunction->getAttributes().getFnAttributes();
  unsigned AttrVal = 0;
  while( AttrSet.hasAttribute("pipename"+std::to_string(AttrVal)) ){
    AttrVal = AttrVal + 1;
  }

  TheFunction->addFnAttr("pipename" + std::to_string(AttrVal),PipeName);
  TheFunction->addFnAttr("pipeline" + std::to_string(AttrVal),PipeLine);

  std::vector<std::string> FoundVect;


  // walk all the pipelines and determine if the current pipe stage
  // is defined across multiple pipelines.  if so, raise an error in the
  // parser and inform the user.
  if( PipeLine.length() > 0 ){
    std::map<std::string, GlobalVariable*>::iterator it;
    it = SCParser::GlobalNamedValues.find(PipeLine);
    if( it != SCParser::GlobalNamedValues.end() ){
      for( auto &Global : SCParser::TheModule->getGlobalList() ){
        AttributeSet PAttrSet = Global.getAttributes();
        if( PAttrSet.hasAttribute("pipeline") ){
          bool done = false;
          unsigned Idx = 0;
          while( !done ){
            if( !PAttrSet.hasAttribute("pipestage" + std::to_string(Idx)) ){
              done = true;
            }else if( PAttrSet.getAttribute("pipestage" + std::to_string(Idx)).getValueAsString().str() == PipeName ){
              FoundVect.push_back(PAttrSet.getAttribute("pipeline").getValueAsString().str());
            }
            Idx = Idx + 1;
          }// end while
        }// end if( PAtterSet.hasAttribute)
      }// end for( Global )
    } // end if( it != end() )
  } // end if( PipeLine.length() )

  if( FoundVect.size() > 1 ){
    return LogErrorV( "Found a collision in pipe stages for pipeline=" +
                      PipeLine + " for pipestage=" + PipeName );
  }

  // build the metadata
  MDNode *N = MDNode::get(SCParser::TheContext,
                          MDString::get(SCParser::TheContext,PipeName));
  MDNode *PN = MDNode::get(SCParser::TheContext,
                          MDString::get(SCParser::TheContext,PipeLine));
  MDNode *TmpPI = MDNode::get(SCParser::TheContext,
                              ConstantAsMetadata::get(ConstantInt::get(
                                SCParser::TheContext,
                                llvm::APInt(64, (uint64_t)(Instance), false))));
  MDNode *PI = MDNode::get(SCParser::TheContext, TmpPI);

  // assign to the global copies such that the other codegen
  // functions can pick up the necessary MDNode's
  SCParser::NameMDNode = N;
  SCParser::InstanceMDNode = PI;
  SCParser::PipelineMDNode = PN;

  // Walk all the body instructions, emit them and tag each
  // instruction with the appropriate metadata
  for( unsigned i=0; i<BodyExpr.size(); i++ ){
    Value *BVal = BodyExpr[i]->codegen();
    if( isa<Instruction>(*BVal) ){
      Instruction *Inst = cast<Instruction>(BVal);
      Inst->setMetadata("pipe.pipeName",N);
      Inst->setMetadata("pipe.pipeLine",PN);
      Inst->setMetadata("pipe.pipeInstance",PI);
    }
  }

  // exit the pipe codegen block
  SCParser::NameMDNode = nullptr;
  SCParser::InstanceMDNode = nullptr;
  SCParser::PipelineMDNode = nullptr;

  // pipe expr
  return Constant::getNullValue(Type::getIntNTy(SCParser::TheContext,64));
}

Value *DoWhileExprAST::codegen() {
  // Retrieve the function parent
  Function *TheFunction = SCParser::Builder.GetInsertBlock()->getParent();

  // Retrieve a unique local label
  unsigned LocalLabel = GetLocalLabel();

  // Make the new basic block for the loop header, inserting after current
  // block.
  // loop entry block
  BasicBlock *LoopBB = BasicBlock::Create(SCParser::TheContext,
                                          "entrydowhile."+std::to_string(LocalLabel),
                                          TheFunction);

  // Insert an explicit fall through from the current block to the LoopBB.
  Builder.CreateBr(LoopBB);

  // Start insertion in LoopBB.
  Builder.SetInsertPoint(LoopBB);

  // Emit the body of the loop.  This, like any other expr, can change the
  // current BB.  Note that we ignore the value computed by the body, but don't
  // allow an error.
  if( BodyExpr.size() == 0 ) return nullptr;
  for( unsigned i=0; i<BodyExpr.size(); i++ ){
    BodyExpr[i]->codegen();
  }

  // Emit the while conditional block
  Value *CondV = Cond->codegen();
  if (!CondV){
    return nullptr;
  }

  // loop exit block
  BasicBlock *AfterBB =
      BasicBlock::Create(SCParser::TheContext,
                         "afterdowhileloop." + std::to_string(LocalLabel),
                         TheFunction);

  // Insert the conditional branch for the initial state
  BranchInst *BI = Builder.CreateCondBr(CondV,LoopBB,AfterBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // Anything new is inserted after the while exit
  Builder.SetInsertPoint(AfterBB);

  // while expr always returns 0
  return Constant::getNullValue(Type::getIntNTy(SCParser::TheContext,64));
}

Value *WhileExprAST::codegen() {
  // Retrieve the function parent
  Function *TheFunction = SCParser::Builder.GetInsertBlock()->getParent();

  // Retrieve a unique local label
  unsigned LocalLabel = GetLocalLabel();

  // Make the new basic block for the loop header, inserting after current
  // block.
  // loop entry block
  BasicBlock *LoopBB = BasicBlock::Create(SCParser::TheContext,
                                          "entrywhile."+std::to_string(LocalLabel),
                                          TheFunction);

  // Insert an explicit fall through from the current block to the LoopBB.
  BranchInst *BI = Builder.CreateBr(LoopBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // loop body block
  BasicBlock *EntryBB = BasicBlock::Create(SCParser::TheContext,
                                          "while."+std::to_string(LocalLabel),
                                          TheFunction);

  // Emit the body of the loop.  This, like any other expr, can change the
  // current BB.  Note that we ignore the value computed by the body, but don't
  // allow an error.
  Builder.SetInsertPoint(EntryBB);
  if( BodyExpr.size() == 0 ) return nullptr;
  for( unsigned i=0; i<BodyExpr.size(); i++ ){
    BodyExpr[i]->codegen();
  }

  // Insert an explicit branch back
  Builder.CreateBr(LoopBB);

  // loop exit block
  BasicBlock *AfterBB =
      BasicBlock::Create(SCParser::TheContext,
                         "afterwhileloop." + std::to_string(LocalLabel),
                         TheFunction);

  // Start insertion in LoopBB.
  Builder.SetInsertPoint(LoopBB);

  // Emit the initial conditional block
  Value *CondV = Cond->codegen();
  if (!CondV){
    return nullptr;
  }

  // Insert the conditional branch for the initial state
  BI = Builder.CreateCondBr(CondV,EntryBB,AfterBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // Anything new is inserted after the while exit
  Builder.SetInsertPoint(AfterBB);

  // while expr always returns 0
  return Constant::getNullValue(Type::getIntNTy(SCParser::TheContext,64));
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
  StoreInst *SI = SCParser::Builder.CreateStore(StartVal,Alloca);
  if( SCParser::NameMDNode ){
    SI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    SI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    SI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  unsigned LocalLabel = GetLocalLabel();

  // Make the new basic block for the loop header, inserting after current
  // block.
  BasicBlock *LoopBB = BasicBlock::Create(SCParser::TheContext,
                                          "loop."+std::to_string(LocalLabel),
                                          TheFunction);

  // Insert an explicit fall through from the current block to the LoopBB.
  BranchInst *BI = Builder.CreateBr(LoopBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }


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
    StepVal = ConstantInt::get(SCParser::TheContext, APInt(64,0,false));
  }

#if 0
  Value *CurVar  = Builder.CreateLoad(Alloca,VarName.c_str());
  Value *NextVar = Builder.CreateAdd(CurVar,
                                      StepVal,
                                      "nextvar" + std::to_string(LocalLabel));
#endif
  // Compute the end condition.
  Value *EndCond = End->codegen();
  if (!EndCond)
    return nullptr;

  // reload and increment the alloca
  Value *CurVar  = Builder.CreateLoad(Alloca,VarName.c_str());
  Value *NextVar = Builder.CreateAdd(CurVar,
                                      StepVal,
                                      "nextvar" + std::to_string(LocalLabel));

  SI = Builder.CreateStore(NextVar,Alloca);
  if( SCParser::NameMDNode ){
    cast<LoadInst>(CurVar)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    cast<LoadInst>(CurVar)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    cast<LoadInst>(CurVar)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    cast<Instruction>(NextVar)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    cast<Instruction>(NextVar)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    cast<Instruction>(NextVar)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    SI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    SI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    SI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }


  // Convert condition to a bool by comparing non-equal to 0.0.
  //EndCond = Builder.CreateFCmpONE(
  EndCond = Builder.CreateICmpNE(
      EndCond, ConstantInt::get(SCParser::TheContext,
                               APInt(1,0,false)),
                               "loopcond" + std::to_string(LocalLabel));
  if( SCParser::NameMDNode ){
    cast<Instruction>(EndCond)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    cast<Instruction>(EndCond)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    cast<Instruction>(EndCond)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // Create the "after loop" block and insert it.
  BasicBlock *AfterBB =
      BasicBlock::Create(SCParser::TheContext,
                         "afterloop." + std::to_string(LocalLabel),
                         TheFunction);

  // Insert the conditional branch into the end of LoopEndBB.
  BI = Builder.CreateCondBr(EndCond, LoopBB, AfterBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // Any new code will be inserted in AfterBB.
  Builder.SetInsertPoint(AfterBB);

  // Restore the unshadowed variable.
  if (OldVal)
    NamedValues[VarName] = OldVal;
  else
    NamedValues.erase(VarName);

  // for expr always returns 0
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

  Value *LI = SCParser::Builder.CreateLoad(V,Name.c_str());
  if( SCParser::NameMDNode ){
    cast<LoadInst>(LI)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    cast<LoadInst>(LI)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    cast<LoadInst>(LI)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }
  return LI;
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
    StoreInst *SI = Builder.CreateStore(InitVal, Alloca);
    if( SCParser::NameMDNode ){
      SI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      SI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      SI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }

    OldBindings.push_back(SCParser::NamedValues[VarName]);
    SCParser::NamedValues[VarName] = Alloca;
  }

  // Codegen the body, now that all vars are in scope.
  Value *BodyVal = Body->codegen();
  if (!BodyVal)
    return nullptr;

  // Pop all our variables from scope.
#if 0
  for (unsigned i = 0, e = VarNames.size(); i != e; ++i)
    SCParser::NamedValues[VarNames[i].first] = OldBindings[i];
#endif

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

    StoreInst *SI = Builder.CreateStore(Val, Variable);
    if( SCParser::NameMDNode ){
      SI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      SI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      SI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }

    return Val;
  }

  // handle the remainder of the binary operators
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R){
    return nullptr;
  }

  // interrogate the types of the operands, mutate the operands if necessary
  Type *LT = L->getType();
  Type *RT = R->getType();

  // test for type mismatches
  if( LT->getTypeID() != RT->getTypeID() ){
    // type mismatches
    if( LT->isFloatingPointTy() ){
      // LHS is the float
      R = SCParser::Builder.CreateUIToFP(R,LT,"uitofptmp");
      if( SCParser::NameMDNode ){
        cast<Instruction>(R)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
        cast<Instruction>(R)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
        cast<Instruction>(R)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
      }
    }else{
      // RHS is the float
      R = SCParser::Builder.CreateFPToUI(R,LT,"fptouitmp");
      if( SCParser::NameMDNode ){
        cast<Instruction>(R)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
        cast<Instruction>(R)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
        cast<Instruction>(R)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
      }
    }
  }else if( LT->getIntegerBitWidth() != RT->getIntegerBitWidth() ){
    // integer type mismatch, cast to the size of the RHS
    R = SCParser::Builder.CreateIntCast(R,LT,true);
    if( SCParser::NameMDNode ){
      cast<Instruction>(R)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      cast<Instruction>(R)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      cast<Instruction>(R)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }
  }

  Value *VI = nullptr;

  if( LT->isFloatingPointTy() ){
    // float ops
    switch (Op) {
    case '+':
      VI = SCParser::Builder.CreateFAdd(L, R, "addtmp");
      break;
    case '-':
      VI = SCParser::Builder.CreateFSub(L, R, "subtmp");
      break;
    case '*':
      VI = SCParser::Builder.CreateFMul(L, R, "multmp");
      break;
    case '<':
      VI = SCParser::Builder.CreateFCmpULT(L, R, "cmptmp");
      break;
    case '>':
      VI = SCParser::Builder.CreateFCmpUGT(L, R, "cmptmp");
      break;
    case '%':
      VI = SCParser::Builder.CreateFRem(L, R, "modtmp");
      break;
    case '/':
      VI = SCParser::Builder.CreateFDiv(L, R, "divtmp", nullptr);
      break;
    case '&':
      VI = SCParser::Builder.CreateAnd(L, R, "andtmp" );
      break;
    case '|':
      VI = SCParser::Builder.CreateOr(L, R, "ortmp" );
      break;
    case '^':
      VI = SCParser::Builder.CreateXor(L, R, "xortmp" );
      break;
    case dyad_shfl:
      L = SCParser::Builder.CreateShl(L, R, "shfltmp", false, false );
      VI = SCParser::Builder.CreateUIToFP(L, R->getType(), "booltmp");
      break;
    case dyad_shfr:
      VI = SCParser::Builder.CreateLShr(L, R, "lshfrtmp", false );
      break;
    case dyad_eqeq:
      VI = SCParser::Builder.CreateFCmpUEQ(L, R, "cmpeq" );
      break;
    case dyad_noteq:
      VI = SCParser::Builder.CreateFCmpUNE(L, R, "cmpeq" );
      break;
    case dyad_logand:
      VI = SCParser::Builder.CreateAnd(L, R, "andtmp" );
      break;
    case dyad_logor:
      VI = SCParser::Builder.CreateOr(L, R, "ortmp" );
      break;
    case dyad_gte:
      VI = SCParser::Builder.CreateFCmpUGE(L, R, "cmptmp");
      break;
    case dyad_lte:
      VI = SCParser::Builder.CreateFCmpULE(L, R, "cmptmp");
      break;
    default:
      return LogErrorV("invalid binary operator");
    }
  }else{
    // integer ops
    switch (Op) {
    case '+':
      VI = SCParser::Builder.CreateAdd(L, R, "addtmp");
      break;
    case '-':
      VI = SCParser::Builder.CreateSub(L, R, "subtmp");
      break;
    case '*':
      VI = SCParser::Builder.CreateMul(L, R, "multmp");
      break;
    case '<':
      VI = SCParser::Builder.CreateICmpULT(L, R, "cmptmp");
      break;
    case '>':
      VI = SCParser::Builder.CreateICmpUGT(L, R, "cmptmp");
      break;
    case '%':
      VI = SCParser::Builder.CreateURem(L, R, "modtmp");
      break;
    case '/':
      VI = SCParser::Builder.CreateUDiv(L, R, "divtmp", true);
      break;
    case '&':
      VI = SCParser::Builder.CreateAnd(L, R, "andtmp" );
      break;
    case '|':
      VI = SCParser::Builder.CreateOr(L, R, "ortmp" );
      break;
    case '^':
      VI = SCParser::Builder.CreateXor(L, R, "xortmp" );
      break;
    case dyad_shfl:
      VI = SCParser::Builder.CreateShl(L, R, "shfltmp", false, false );
      break;
    case dyad_shfr:
      VI = SCParser::Builder.CreateLShr(L, R, "lshfrtmp", false );
      break;
    case dyad_eqeq:
      VI = SCParser::Builder.CreateICmpEQ(L, R, "cmpeq" );
      break;
    case dyad_noteq:
      VI = SCParser::Builder.CreateICmpNE(L, R, "cmpne" );
      break;
    case dyad_logand:
      VI = SCParser::Builder.CreateAnd(L, R, "andtmp" );
      break;
    case dyad_logor:
      VI = SCParser::Builder.CreateOr(L, R, "ortmp" );
      break;
    case dyad_gte:
      VI = SCParser::Builder.CreateICmpUGE(L, R, "cmptmp");
      break;
    case dyad_lte:
      VI = SCParser::Builder.CreateICmpULE(L, R, "cmptmp");
      break;
    default:
      return LogErrorV("invalid binary operator");
    }
  }

  if( SCParser::NameMDNode ){
    cast<Instruction>(VI)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    cast<Instruction>(VI)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    cast<Instruction>(VI)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  return VI;

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

  Value *CI = SCParser::Builder.CreateCall(CalleeF, ArgsV, "calltmp");
  if( SCParser::NameMDNode ){
    cast<Instruction>(CI)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    cast<Instruction>(CI)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    cast<Instruction>(CI)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }
  return CI;
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

  if( InstFormat.length() > 0 ){
    F->addFnAttr("instformat",InstFormat);
  }

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

Value *InstFormatAST::codegen(){
  // instruction fields
  Type *VType = Type::getIntNTy(SCParser::TheContext,64); // create a generic uint64_t
  std::vector<std::tuple<std::string,SCInstField,std::string,unsigned>>::iterator it;

  for( it=Fields.begin(); it != Fields.end(); ++it ){
    std::string FName = std::get<0>(*it);
    SCInstField FT = std::get<1>(*it);
    std::string RClass = std::get<2>(*it);
    unsigned Width = std::get<3>(*it);

    // search the GlobalNamedValues map for the target variable name
    // if its not found, then create an entry
    std::map<std::string, GlobalVariable*>::iterator GVit;
    GVit = GlobalNamedValues.find(FName);

    if( GVit == GlobalNamedValues.end() ){
      // variable is not in the global list, create a new one
      GlobalVariable *val = new GlobalVariable(*SCParser::TheModule,
                                              VType,
                                              false,
                                              GlobalValue::ExternalLinkage,
                                              nullptr,
                                              Twine(FName),
                                              nullptr,
                                              GlobalVariable::NotThreadLocal,
                                              0);
      if( !val ){
        return LogErrorV( "Failed to lower instruction format field to global: instformat = " + 
                          Name + " field=" + FName);
      }

      // add the global to the top-level names list
      GlobalNamedValues[FName] = val;

      // add the field name to differentiate across other fields
      val->addAttribute("field_name",FName);

      // add an attribute to track the value to the instruction format
      val->addAttribute("instformat0",Name);

      // add an attribute to track the value of the instruction field width
      val->addAttribute("instwidth0",std::to_string(Width));

      // add attributes for the field type
      switch( FT ){
      case field_enc:
        val->addAttribute("fieldtype","encoding");
        break;
      case field_reg:
        val->addAttribute("fieldtype","register");
        val->addAttribute("regclasscontainer0",RClass);
        break;
      case field_imm:
        val->addAttribute("fieldtype","immediate");
        break;
      default:
        return LogErrorV( "Failed to lower instruction format field to global: instformat = " + 
                          Name + " field=" + FName);
        break;
      }
    }else{
      // variable is in the global list, add to existing entry
      AttributeSet AttrSet = GVit->second->getAttributes();

      // ensure that the instruction fieldtype is the same
      std::string FType = AttrSet.getAttribute("fieldtype").getValueAsString().str();

      switch( FT ){
      case field_enc:
        if( FType != "encoding" ){
          return LogWarnV( "FieldType for field=" + FName + " in instformat=" +
                            Name + " does not match existing field type from adjacent format");
        }
        break;
      case field_reg:
        if( FType != "register"){
          return LogWarnV( "FieldType for field=" + FName + " in instformat=" +
                            Name + " does not match existing field type from adjacent format");
        }
        break;
      case field_imm:
        if( FType != "immediate" ){
          return LogWarnV( "FieldType for field=" + FName + " in instformat=" +
                            Name + " does not match existing field type from adjacent format");
        }
        break;
      default:
        return LogWarnV( "Failed to lower instruction format field to global: instformat = " + 
                          Name + " field=" + FName + "; Does not match existing fieldtype");
        break;
      }

      // derive the number of instruction formats
      unsigned NextIF = 0;
      while( AttrSet.hasAttribute("instformat"+std::to_string(NextIF)) ){
        NextIF = NextIF+1;
      }

      // insert the new instruction format
      GVit->second->addAttribute("instformat"+std::to_string(NextIF),Name);

      // insert the new instruction width
      GVit->second->addAttribute("instwidth"+std::to_string(NextIF),std::to_string(Width));

      // insert a new register class type if required
      if( FT == field_reg ){
        GVit->second->addAttribute("regclasscontainer"+std::to_string(NextIF),RClass);
      }
    }
  }
  return nullptr;
}

Value *PipelineAST::codegen(){
  // create a global variable to track the pipeline def
  GlobalVariable *val = new GlobalVariable(*SCParser::TheModule,
                                           Type::getIntNTy(SCParser::TheContext,64),
                                           false,
                                           GlobalValue::ExternalLinkage,
                                           nullptr,
                                           Twine(Name),
                                           nullptr,
                                           GlobalVariable::NotThreadLocal,
                                           0);
  if( !val )
    return LogErrorV( "Failed to lower pipeline definition to global: pipeline = " + Name);

  GlobalNamedValues[Name] = val;

  // add all the attributes
  val->addAttribute("pipeline", Name);
  for( unsigned i=0; i<Attrs.size(); i++ ){
    val->addAttribute("pipeattr" + std::to_string(i), Attrs[i]);
  }

  return nullptr;
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

      if( PC == Args[i] ){
        val->addAttribute("pc", "true");
      }
      if( Attrs[i].defReadOnly ){
        val->addAttribute("readOnly", "true");
      }
      if( Attrs[i].defReadWrite ){
        val->addAttribute("readWrite", "true");
      }
      if( Attrs[i].defCSR ){
        val->addAttribute("csr", "true");
      }
      if( Attrs[i].defAMS ){
        val->addAttribute("ams", "true");
      }
      if( Attrs[i].defTUS ){
        val->addAttribute("tus", "true");
      }
      if( Attrs[i].defShared ){
        val->addAttribute("shared", "true");
      }
      // if its a matrix reg
      if( Attrs[i].defMatrix ){
        if( Attrs[i].defElem ){
          val->addAttribute("xidx", std::to_string(Attrs[i].xIdx));
          val->addAttribute("yidx", std::to_string(Attrs[i].yIdx));
          val->addAttribute("containers", Attrs[i].containers);
        }
        // if vector within matrix
        else if( Attrs[i].defVector ){
          val->addAttribute("vector", "true");
          val->addAttribute("containers", Attrs[i].containers);
          // differentiate row / col
          if( Attrs[i].xIdx + Attrs[i].yIdx == -2 ){
            continue;
          }
          else if( Attrs[i].yIdx == -1 ){
            val->addAttribute("xidx", std::to_string(Attrs[i].xIdx));
            val->addAttribute("xdim", std::to_string(Attrs[i].elems));
          }
          else{
            val->addAttribute("yidx", std::to_string(Attrs[i].yIdx));
            val->addAttribute("ydim", std::to_string(Attrs[i].elems2D));
          }
          // val->addAttribute("elements", std::to_string(elems));
        }
        else{ 
          val->addAttribute("matrix", "true");
          val->addAttribute("xdim", std::to_string(Attrs[i].elems));
          val->addAttribute("ydim", std::to_string(Attrs[i].elems2D));
        }
      }
      // if its a vector reg
      if( Attrs[i].defVector && !Attrs[i].defMatrix ){
        // if its a vector element
        if( Attrs[i].defElem ){
          val->addAttribute("xidx", std::to_string(Attrs[i].xIdx));
          val->addAttribute("containers", Attrs[i].containers);
        }else{
          // not an element
          val->addAttribute("xdim", std::to_string(Attrs[i].elems));
          val->addAttribute("vector", "true");
        }
      }
      
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
    StoreInst *SI = Builder.CreateStore(&Arg, Alloca);
    if( SCParser::NameMDNode ){
      SI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      SI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      SI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }

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

  CondV = Builder.CreateICmpNE(
        CondV, ConstantInt::get(SCParser::TheContext, APInt(1,0,false)),
        "ifcond."+std::to_string(LocalLabel));
  if( SCParser::NameMDNode ){
    cast<Instruction>(CondV)->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    cast<Instruction>(CondV)->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    cast<Instruction>(CondV)->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

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

  BranchInst *BI = Builder.CreateCondBr(CondV, ThenBB, ElseBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // Emit then value.
  Builder.SetInsertPoint(ThenBB);

  // Emit all the Then body values
  Value *TV = nullptr;
  for( unsigned i=0; i<ThenV.size(); i++ ){
    TV = ThenV[i]->codegen();
    if( !TV )
      return nullptr;
  }

  BI = Builder.CreateBr(MergeBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = Builder.GetInsertBlock();

  // Emit else block.
  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);


  Value *EV = nullptr;
  for( unsigned i=0; i<ElseV.size(); i++ ){
    EV = ElseV[i]->codegen();
    if( !EV )
      return nullptr;
  }

  BI = Builder.CreateBr(MergeBB);
  if( SCParser::NameMDNode ){
    BI->setMetadata("pipe.pipeName",SCParser::NameMDNode);
    BI->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
    BI->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
  }

  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = Builder.GetInsertBlock();

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);

  PHINode *PN = nullptr;
  if( TV->getType()->isFloatingPointTy() ){
    PN = Builder.CreatePHI(TV->getType(),
                                    2, "iftmp."+std::to_string(LocalLabel));
    if( SCParser::NameMDNode ){
      PN->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      PN->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      PN->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }
  }else{
    PN = Builder.CreatePHI(TV->getType(),
                                    2, "iftmp."+std::to_string(LocalLabel));
    if( SCParser::NameMDNode ){
      PN->setMetadata("pipe.pipeName",SCParser::NameMDNode);
      PN->setMetadata("pipe.pipeLine",SCParser::PipelineMDNode);
      PN->setMetadata("pipe.pipeInstance",SCParser::InstanceMDNode);
    }
  }

  PN->addIncoming(TV, ThenBB);
  if( EV != nullptr ){
    // check to see if we need to adjust the size of the types
    if( TV->getType()->getPrimitiveSizeInBits() !=
        EV->getType()->getPrimitiveSizeInBits() ){
      EV->mutateType(TV->getType());
    }
    PN->addIncoming(EV, ElseBB);
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

std::unique_ptr<PipelineAST> SCParser::LogErrorPI(std::string Str){
  LogError(Str);
  return nullptr;
}

std::unique_ptr<InstFormatAST> SCParser::LogErrorIF(std::string Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<FunctionAST> SCParser::LogErrorF(std::string Str) {
  LogError(Str);
  return nullptr;
}

Value *LogWarnV(std::string Str){
  SCParser::GMsgs->PrintMsg( L_WARN, Str );
  return nullptr;
}

Value *LogErrorV(std::string Str){
  SCParser::GMsgs->PrintMsg( L_ERROR, Str );
  return nullptr;
}

// EOF
