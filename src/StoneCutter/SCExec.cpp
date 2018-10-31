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

  // remove the stale ouptput file
  if( Opts->GetOutputFile().length() > 0 ){
    if( SCFileExists(Opts->GetOutputFile()) ){
      SCDeleteFile(Opts->GetOutputFile());
    }
  }

  // Init a new parser
  Parser = new SCParser( Msgs );

  // Do we execute the optimizer on the IR?
  if( !Opts->IsOptimize() ){
    if( !Parser->Optimize() ){
      Msgs->PrintMsg( L_ERROR, "Failed to initialize the optimizer" );
      delete Parser;
      return false;
    }
  }

  // for each file, read it into a buffer and parse it
  // accordingly
  // we utilize a common output file
  for( unsigned i=0; i<Opts->GetNumInputFiles(); i++ ){

    // Read the file into a buffer
    std::ifstream t(Opts->GetInputFile(i));
    std::string Buf( (std::istreambuf_iterator<char>(t)),
                     (std::istreambuf_iterator<char>()));

    if( !Parser->SetInputs(Buf,Opts->GetInputFile(i)) ){
      Msgs->PrintMsg( L_ERROR, "Failed to set parser input" );
      delete Parser;
      return false;
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
#if 0

    SCLLCodeGen *CG;

    // Do we execute the LLVM IR codegen?
    if( Opts->IsIR() ){
      // if the output file name is null,
      // we print to stdout
      CG = new SCLLCodeGen(Parser,Msgs,Opts->GetOutputFile());
      if( !CG->GenerateLL() ){
        Msgs->PrintMsg( L_ERROR, "Failed to generate IR for " +
                        Opts->GetInputFile(i) );
        delete CG;
        return false;
      }
    }else{
      // else, we return now
      return true;
    }
#endif
  }


  // Do we execute the IR codegen?
  if( Opts->IsIR() ){
    SCLLCodeGen *CG = new SCLLCodeGen(Parser,Msgs,Opts->GetOutputFile());
    if( !CG->GenerateLL() ){
      Msgs->PrintMsg( L_ERROR, "Failed to generate LLVM IR" );
      delete CG;
      delete Parser;
    }
  }

  // Do we toss the intermediate files?
  if( !Opts->IsKeep() ){
  }

  delete Parser;
  return true;
}

// EOF
