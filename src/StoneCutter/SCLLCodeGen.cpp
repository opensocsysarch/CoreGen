//
// _SCLLCodeGen_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCLLCodeGen.h"

SCLLCodeGen::SCLLCodeGen( SCParser *P, SCMsg *M, std::string OF )
  : Parser(P), Msgs(M), OutputFile(OF) {
}

SCLLCodeGen::~SCLLCodeGen(){
}

bool SCLLCodeGen::GenerateLL(){

  if( !Parser ){
    return false;
  }

  SCParser::TheModule->print(errs(),nullptr);

  return true;
}

// EOF
