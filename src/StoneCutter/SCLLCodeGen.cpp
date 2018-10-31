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

  if( OutputFile.length() == 0 ){
    // output to stdout
    SCParser::TheModule->print(outs(),nullptr);
  }else{
    StringRef FileName(OutputFile);
    std::error_code EC;
    raw_fd_ostream OS(FileName,EC,llvm::sys::fs::F_Append);
    SCParser::TheModule->print(OS,nullptr);
  }

  return true;
}

// EOF
