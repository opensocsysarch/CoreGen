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

SCParser::SCParser(std::string B, std::string F, SCMsg *M)
  : InBuf(B), FileName(F), Msgs(M), Lex(new SCLexer(B)) {
    InitBinopPrecedence();
}

SCParser::~SCParser(){
}

void SCParser::InitBinopPrecedence(){
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
}

bool SCParser::Parse(){
  if( InBuf.length() == 0 ){
    Msgs->PrintMsg( L_ERROR, "No StoneCutter input defined");
    return false;
  }
  return true;
}

// EOF
