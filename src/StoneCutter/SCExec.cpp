//
// _SCExec_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCExec.h"

// ------------------------------------------------- CONSTRUCTOR
SCExec::SCExec( SCOpts *O, SCMsg *M )
  : Opts(O), Msgs(M){
}

// ------------------------------------------------- DESTRUCTOR
SCExec::~SCExec(){}

// ------------------------------------------------- EXEC
bool SCExec::Exec(){

  // for each file, read it into a buffer and parse it
  // accordingly
  for( unsigned i=0; i<Opts->GetNumInputFiles(); i++ ){

    // Read the file into a buffer
    std::string Buf;
    Parser = new SCParser( Buf, Opts->GetInputFile(i), Msgs );

    // We always want to execute the parser, if not, we fail
    // this may change in the future where we can read in
    // IR for code generation
    if( Opts->IsParse() ){
      // execute the parser
    }else{
      // we have failed
      delete Parser;
      return false;
    }

    // Do we execute the LLVM IR codegen?
    if( Opts->IsIR() ){
      // execute the IR codegen
    }else{
      // else, we return now
      return true;
    }

    // Do we execute the optimizer on the IR?
    if( Opts->IsOptimize() ){
      // execute the optimizer
    }// keep going, we may be running -O0

    // Do we execute the Chisel codegen?
    if( Opts->IsCG() ){
      // Execute the Chisel codegen
    }

    // Do we toss the intermediate files?
    if( !Opts->IsKeep() ){
    }

    delete Parser;
  }

  // We always want to execute the parser, if not, we fail
  // this may change in the future where we can read in
  // IR for code generation
  if( Opts->IsParse() ){
    // execute the parser
  }else{
    // we have failed
    return false;
  }

  return true;
}

// EOF
