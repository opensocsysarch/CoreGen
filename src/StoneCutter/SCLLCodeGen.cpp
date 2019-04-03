//
// _SCLLCodeGen_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCLLCodeGen.h"

SCLLCodeGen::SCLLCodeGen( SCParser *P, SCMsg *M,
                          std::string OF, std::string OBJF )
  : Parser(P), Msgs(M), OutputFile(OF), ObjFile(OBJF) {
}

SCLLCodeGen::~SCLLCodeGen(){
}

bool SCLLCodeGen::GenerateLL(){

  if( !Parser ){
    return false;
  }

  // this should probably be deprecated
  if( OutputFile.length() == 0 ){
    // output to stdout
    SCParser::TheModule->print(outs(),nullptr);
  }else{
    // output to file
    StringRef FileName(OutputFile);
    std::error_code EC;
    raw_fd_ostream OS(FileName,EC,llvm::sys::fs::F_None);
    SCParser::TheModule->print(OS,nullptr);
  }

  return true;
}

bool SCLLCodeGen::GenerateObjFile(){
  if( !Parser ){
    return false;
  }

  if( ObjFile.length() == 0 ){
    return false;
  }

  // init all the target info
  auto TargetTriple = sys::getDefaultTargetTriple();
  LLVM_NATIVE_TARGETINFO();
  LLVM_NATIVE_TARGET();
  LLVM_NATIVE_TARGETMC();
  LLVM_NATIVE_ASMPARSER();
  LLVM_NATIVE_ASMPRINTER();

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  if( !Target ){
    Msgs->PrintMsg( L_ERROR, "Could not init the default target : " + Error );
    return false;
  }

  auto CPU = "generic";
  auto Features = "";

  TargetOptions opt;
  auto RM = Optional<Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  SCParser::TheModule->setDataLayout(TheTargetMachine->createDataLayout());

  // emit the object file
  std::error_code EC;
  raw_fd_ostream dest(StringRef(ObjFile), EC, sys::fs::F_None);
  if( EC ){
    Msgs->PrintMsg( L_ERROR, "Could not open file: " + ObjFile );
    return false;
  }

  legacy::PassManager pass;
  auto FileType = TargetMachine::CGFT_ObjectFile;
  if (TheTargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
    Msgs->PrintMsg( L_ERROR, "Could not emit a file of this type" );
    return false;
  }
  pass.run(*SCParser::TheModule);
  dest.flush();

  return true;
}

// EOF
