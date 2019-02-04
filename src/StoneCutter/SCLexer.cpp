//
// _SCLexer_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCLexer.h"

SCLexer::SCLexer() : LineNum(1), CurChar(0) {
}

SCLexer::~SCLexer(){
}

bool SCLexer::SetInput(std::string B){
  if( B.length() == 0 ){
    return false;
  }
  InBuf = B;
  return true;
}

int SCLexer::GetNext(){
  if( (unsigned)(CurChar) >= InBuf.size() ){
    return EOF;
  }
  int TChar = InBuf[CurChar];
  CurChar++;
  if( TChar == '\n' ){
    LineNum++;
  }
  return TChar;
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

bool SCLexer::IsVarDef(){
  int Idx = 0;

  // common types
  while( VarAttrEntryTable[Idx].Name != "." ){
    if( IdentifierStr == VarAttrEntryTable[Idx].Name ){
      Var.width     = VarAttrEntryTable[Idx].width;
      Var.elems     = VarAttrEntryTable[Idx].elems;
      Var.defSign   = VarAttrEntryTable[Idx].IsDefSign;
      Var.defVector = VarAttrEntryTable[Idx].IsDefVector;
      Var.defFloat  = VarAttrEntryTable[Idx].IsDefFloat;
      Var.defRegClass = false;
      return true;
    }
    Idx++;
  }

  // all other types
  if( IdentifierStr[0] == 'u' ){
    // unsigned variable
    Var.elems     = 1;
    Var.defSign   = false;
    Var.defVector = false;
    Var.defFloat  = false;
    Var.defRegClass = false;
    Var.width     = std::stoi( IdentifierStr.substr(1,IdentifierStr.length()-1) );
    return true;
  }else if( IdentifierStr[0] == 's' ){
    // signed variable
    Var.elems = 1;
    Var.defSign   = true;
    Var.defVector = false;
    Var.defFloat  = false;
    Var.defRegClass = false;
    Var.width     = std::stoi( IdentifierStr.substr(1,IdentifierStr.length()-1) );
    return true;
  }

  return false;
}

int SCLexer::IsValidChar(int *LastChar){
  int TmpChar = GetNext();
  *LastChar = TmpChar;
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

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = GetNext();

  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    //while (isalnum((LastChar = GetNext()))){
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
    if( IsVarDef() )
      return tok_var;
    if( IsIntrinsic() )
      return tok_intrin;

    // the identifier
    return tok_identifier;
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
