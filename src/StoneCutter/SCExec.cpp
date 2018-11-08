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
  // we utilize a common output file
  for( unsigned i=0; i<Opts->GetNumInputFiles(); i++ ){

    // Read the file into a buffer
    std::ifstream t(Opts->GetInputFile(i));
    std::string Buf( (std::istreambuf_iterator<char>(t)),
                     (std::istreambuf_iterator<char>()));

    Parser = new SCParser(Buf,Opts->GetInputFile(i),Msgs);
    if( !Parser ){
      Msgs->PrintMsg( L_ERROR, "Failed to initiate the StoneCutter parser" );
      return false;
    }

    // Do we execute the inline optimizer?
    if( Opts->IsOptimize() ){
      if( !Parser->Optimize() ){
        Msgs->PrintMsg( L_ERROR, "Failed to initialize the optimizer" );
        delete Parser;
        return false;
      }
    }

    // We always want to execute the parser, if not, we fail
    // this may change in the future where we can read in
    // IR for code generation
    if( Opts->IsParse() ){
      // execute the parser
      if( !Parser->Parse() ){
        Msgs->PrintMsg( L_ERROR, "Failed to parse " + Opts->GetInputFile(i) );
        delete Parser;
        return false;
      }
    }else{
      // we have failed
      delete Parser;
      return false;
    }

    SCLLCodeGen *CG;

    // Do we execute the LLVM IR codegen?
    if( Opts->IsIR() ){
      // prep a new output file name; append '.ll'
      CG = new SCLLCodeGen(Parser,Msgs,
                           Opts->GetInputFile(i) + ".ll",
                           Opts->GetInputFile(i) + ".o");
      if( !CG->GenerateLL() ){
        Msgs->PrintMsg( L_ERROR, "Failed to generate IR for " +
                        Opts->GetInputFile(i) );
        delete CG;
        delete Parser;
        return false;
      }
    }

    if( CG ){
      delete CG;
    }
    delete Parser;
  }

  // Do we toss the intermediate files?
  if( !Opts->IsKeep() ){
    // remove all the LL files
    for( unsigned i=0; i<Opts->GetNumInputFiles(); i++ ){
      if( !SCDeleteFile(Opts->GetInputFile(i)+".ll") ){
        Msgs->PrintMsg( L_ERROR, "Failed to delete LLVM IR file " +
                        Opts->GetInputFile(i) + ".ll" );
        return false;
      }
    }
  }

  return true;
}

// EOF
