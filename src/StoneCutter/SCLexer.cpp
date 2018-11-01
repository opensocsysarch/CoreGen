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

SCLexer::SCLexer(std::string B) : InBuf(B), LineNum(1), CurChar(0) {
}

SCLexer::~SCLexer(){
}

int SCLexer::GetNext(){
  if( (unsigned)(CurChar) >= InBuf.size() ){
    return EOF;
  }
  int TChar = InBuf[CurChar];
  CurChar++;
  if( TChar == '\r')
    LineNum++;
  return TChar;
}

int SCLexer::GetTok(){
  static int LastChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = GetNext();

  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = GetNext())))
      IdentifierStr += LastChar;

    // walk all the potential identifiers
    if( IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "inst")
      return tok_inst;
    if (IdentifierStr == "extern")
      return tok_extern;
    if (IdentifierStr == "regclass")
      return tok_regclass;
    if( IdentifierStr == "if" )
      return tok_if;
    if( IdentifierStr == "else" )
      return tok_else;
    if( IdentifierStr == "then" )
      return tok_then;
    if( IdentifierStr == "elseif" )
      return tok_elseif;

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
      LineNum++;
      return GetTok();
    }
    LineNum++;
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = GetNext();
  if( ThisChar == '\r' )
    LineNum++;
  return ThisChar;
}

// EOF
