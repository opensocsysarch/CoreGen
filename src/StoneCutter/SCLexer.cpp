//
// _SCLexer_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCLexer.h"

SCLexer::SCLexer()
  : InBuf(""), IdentifierStr(""), NumVal(0.), LineNum(1), CurChar(0),
    LC(nullptr), isReset(false) {
}

SCLexer::~SCLexer(){
}

void SCLexer::Reset(){
  isReset = true;
}

void SCLexer::InternalReset(){
  *LC = ' ';
  isReset = false;
}

bool SCLexer::SetInput(std::string B){
  if( B.length() == 0 ){
    return false;
  }
  CurChar = 0;
  InBuf = B;
  return true;
}

int SCLexer::GetNext(){
  if( (unsigned)(CurChar) >= InBuf.size() )
    return EOF;

  int TChar = InBuf[CurChar];
  CurChar++;
  if( TChar == '\n' ){
    LineNum++;
  }
  return TChar;
}

int SCLexer::PeekPrev(){
  if( (unsigned)(CurChar) <= 1 ){
    return EOF;
  }
  unsigned LChar = CurChar-2;
  while(isspace(InBuf[LChar])){
    if( LChar == 0 ){
      return EOF;
    }
    LChar = LChar-1;
  }
  return InBuf[LChar];
}

int SCLexer::PeekNext(){
  if( (unsigned)(CurChar) >= InBuf.size() ){
    return EOF;
  }
  return InBuf[CurChar];
}

bool SCLexer::IsIntrinsic(){
  // walk the intrinsic table and determine whether we have any candidates
  return false;
}

bool SCLexer::IsOperator(int LC){
  if( LC == '=' ){
    return true;
  }else if( LC == '+' ){
    return true;
  }else if( LC == '-' ){
    return true;
  }else if( LC == '*' ){
    return true;
  }else if( LC == 92 ){ // divide
    return true;
  }else if( LC == '%' ){
    return true;
  }else if( LC == '|' ){
    return true;
  }else if( LC == '&' ){
    return true;
  }else if( LC == '^' ){
    return true;
  }else if( LC == '>' ){
    return true;
  }else if( LC == '<' ){
    return true;
  }
  return false;
}

bool SCLexer::IsDyadic(int LC){
  if( LC == '=' ){
    return true;
  }else if( LC == '<' ){
    return true;
  }else if( LC == '>' ){
    return true;
  }else if( LC == '!' ){
    return true;
  }else if( LC == '&' ){
    return true;
  }else if( LC == '|' ){
    return true;
  }
  return false;
}

bool SCLexer::IsPipeAttrDef(){
  int Idx = 0;

  // pipeline attributes
  while(PipeAttrTable[Idx] != "."){
    if( IdentifierStr == PipeAttrTable[Idx] )
      return true;
    Idx++;
  }

  if( IdentifierStr.length() < 8 )
    return false;

  std::string Tmp = IdentifierStr.substr(0,7);
  if( Tmp == "stages_" )
    return true;

  return false;
}

bool SCLexer::IsVarDef(){
  int Idx = 0;

  // common types
  while( VarAttrEntryTable[Idx].Name != "." ){
    if( IdentifierStr == VarAttrEntryTable[Idx].Name ){
      Var.width     = VarAttrEntryTable[Idx].width;
      Var.dimX      = VarAttrEntryTable[Idx].dimX;
      Var.dimY      = 0;
      Var.defSign   = VarAttrEntryTable[Idx].IsDefSign;
      Var.defVector = VarAttrEntryTable[Idx].IsDefVector;
      Var.defMatrix = VarAttrEntryTable[Idx].IsDefMatrix;
      Var.defFloat  = VarAttrEntryTable[Idx].IsDefFloat;
      Var.defRegClass = false;
      return true;
    }
    Idx++;
  }

  // make sure this is actually a var def
  if( !isdigit(IdentifierStr[1]) )
    return false;

  // all other types
  if( IdentifierStr[0] == 'u' ){
    // unsigned variable
    Var.dimX      = 1;
    Var.dimY      = 0;
    Var.defSign   = false;
    Var.defVector = false;
    Var.defMatrix = false;
    Var.defFloat  = false;
    Var.defRegClass = false;
    Var.width     = std::stoi( IdentifierStr.substr(1,IdentifierStr.length()-1) );
    return true;
  }else if( IdentifierStr[0] == 's' ){
    // signed variable
    Var.dimX      = 1;
    Var.dimY      = 0;
    Var.defSign   = true;
    Var.defVector = false;
    Var.defMatrix = false;
    Var.defFloat  = false;
    Var.defRegClass = false;
    Var.width     = std::stoi( IdentifierStr.substr(1,IdentifierStr.length()-1) );
    return true;
  }

  return false;
}

int SCLexer::IsValidChar(int *LC){
  int TmpChar = GetNext();
  *LC = TmpChar;
  if( isalnum(TmpChar) ){
    return 1;
  }else if( TmpChar == '_' ){
    return 1;
  }else if( TmpChar == '.' ){
    return 1;
  }
  return 0;
}

int SCLexer::GetTok(){
  static int LastChar = ' ';

  LC = (&LastChar);
  if( isReset ){
    InternalReset();
  }

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = GetNext();

  if ( isalpha(LastChar) || (LastChar == '_') ) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (IsValidChar(&LastChar)){
      IdentifierStr += LastChar;
    }

    // walk all the potential identifiers
    if( IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "inst")
      return tok_inst;
    if (IdentifierStr == "extern")
      return tok_extern;
    if (IdentifierStr == "regclass")
      return tok_regclass;
    if (IdentifierStr == "instformat")
      return tok_instf;
    if( IdentifierStr == "if" )
      return tok_if;
    if( IdentifierStr == "else" )
      return tok_else;
    if( IdentifierStr == "elseif" )
      return tok_elseif;
    if( IdentifierStr == "for" )
      return tok_for;
    if( IdentifierStr == "while" )
      return tok_while;
    if( IdentifierStr == "do" )
      return tok_do;
    if( IdentifierStr == "pipe" )
      return tok_pipe;
    if( IdentifierStr == "pipeline" )
      return tok_pipeline;
    if( IsVarDef() )
      return tok_var;
    if( IsIntrinsic() )
      return tok_intrin;
    if( IsPipeAttrDef() )
      return tok_pipeattr;

    // the identifier
    return tok_identifier;
  }
// NOTE: toupper() provides case insensitivity for users
  if( LastChar == '0' && toupper(PeekNext()) == 'X' ){
    std::string HexNumStr;
    // for a hex formatted number 0x...., consume the prefix
    LastChar = GetNext();  // Eat '0'
    LastChar = GetNext();  // Eat 'x'
    // consume the remainder of the digits [0-9]|[a-F]
    while( isxdigit(LastChar) || LastChar == '_' ){
      HexNumStr += LastChar;
      LastChar = GetNext();
    }
    // If alpha but not hex digit: Invalid
    if( isalpha(LastChar) ){
      ErrorStr = "Invalid Hex: " + InBuf + " @ line " + std::to_string(LineNum);
      return tok_error;
    }
    // Strip HexNumStr of underscores
    HexNumStr.erase(std::remove(HexNumStr.begin(), HexNumStr.end(), '_'), HexNumStr.end()); 
    return tok_number;
  }
  if( LastChar == '0' && toupper(PeekNext()) == 'B' ){
    std::string BinNumStr;
    LastChar = GetNext();  // Eat '0'
    LastChar = GetNext();  // Eat 'b'

    while( LastChar == '0' || LastChar == '1' || LastChar == '_' ){
      BinNumStr += LastChar;
      LastChar = GetNext();
    }
    // If alpha but not hex digit: Invalid
    if( (isdigit(LastChar) && LastChar > 1) || isalpha(LastChar) ){
      ErrorStr = "Invalid Binary: " + InBuf + " @ line " + std::to_string(LineNum);
      return tok_error;
    }
    NumVal = strtoll(BinNumStr.c_str(), nullptr, 2);
    return tok_number;
  }
  if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = GetNext();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }

  // negative numbers
  if ( (LastChar == '-') && IsOperator(PeekPrev()) && (isdigit(PeekNext()) || PeekNext() == '.') ){
    std::string NumStr;
    LastChar = GetNext(); // eat the '-'
    do {
      NumStr += LastChar;
      LastChar = GetNext();
    }while(isdigit(LastChar) || LastChar == '.');

    NumVal = (strtod(NumStr.c_str(), nullptr)*-1);
    return tok_number;
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do{
      LastChar = GetNext();
    }while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF){
      return GetTok();
    }
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;

  // Check for special binary operators [<<,>>,==,!=,&&,||]
  if( IsDyadic(LastChar) && IsDyadic(PeekNext()) ){
    // record the dyadic operator in the identifer string
    // first operator
    IdentifierStr = LastChar;

    // second operator
    LastChar = GetNext();
    IdentifierStr += LastChar;

    LastChar = GetNext();

    // return this as a dyad
    return tok_dyad;
  }

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = GetNext();
  return ThisChar;
}

// EOF
