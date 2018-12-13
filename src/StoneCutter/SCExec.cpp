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


// ------------------------------------------------- PRINTPASSLIST
bool SCExec::PrintPassList(){
  std::string TmpFile = ".parsetmp";
  std::ifstream t(TmpFile);
  std::string Buf( (std::istreambuf_iterator<char>(t)),
                   (std::istreambuf_iterator<char>()));

  Parser = new SCParser(Buf,TmpFile,Msgs);
  if( !Parser ){
    Msgs->PrintMsg( L_ERROR, "Failed to initiate the StoneCutter parser" );
    return false;
  }

  std::vector<std::string> P = Parser->GetPassList();
  Msgs->PrintRawMsg( " StoneCutter LLVM Optimization Passes" );
  Msgs->PrintRawMsg( "--------------------------------------------------------------------------------" );
  for( unsigned i=0; i<P.size(); i++ ){
    Msgs->PrintRawMsg( " -\t" + P[i] );
  }
  Msgs->PrintRawMsg( "--------------------------------------------------------------------------------" );
  delete Parser;
  return true;
}

// ------------------------------------------------- EXEC
bool SCExec::Exec(){

  // Do we want to list the passes and immediately exit? 
  if( Opts->IsListPass() ){
    return PrintPassList();
  }

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
      // do we need to manually enable passes?
      if( Opts->IsEnablePass() ){
        if( !Parser->EnablePasses(Opts->GetEnablePass()) ){
        Msgs->PrintMsg( L_ERROR, "Failed to manually enable optimization passes" );
        delete Parser;
        return false;
        }
      }

      // do we need to manually disable passes?
      if( Opts->IsDisablePass() ){
        if( !Parser->DisablePasses(Opts->GetDisabledPass()) ){
        Msgs->PrintMsg( L_ERROR, "Failed to manually disable optimization passes" );
        delete Parser;
        return false;
        }
      }

      // enable the optimizer
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

    // Do we generate Chisel?
    if( Opts->IsChisel() ){
      if( !Opts->IsIR() ){
        Msgs->PrintMsg( L_ERROR, "LLVM IR is required for Chisel output" );
        delete CG;
        delete Parser;
        return false;
      }

      // TODO, generate the chisel output
    }

    // Do we execute the object codegen
    if( Opts->IsCG() ){
      if( !Opts->IsIR() ){
        Msgs->PrintMsg( L_ERROR, "LLVM IR is required for object files" );
        delete CG;
        delete Parser;
        return false;
      }

      if( !CG->GenerateObjFile() ){
        Msgs->PrintMsg( L_ERROR, "Failed to generate object file for " +
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
    if( Opts->IsIR() ){
      for( unsigned i=0; i<Opts->GetNumInputFiles(); i++ ){
        if( !SCDeleteFile(Opts->GetInputFile(i)+".ll") ){
          Msgs->PrintMsg( L_ERROR, "Failed to delete LLVM IR file " +
                        Opts->GetInputFile(i) + ".ll" );
          return false;
        }
      }
    }
    // remove all the object files
    if( Opts->IsCG() ){
      for( unsigned i=0; i<Opts->GetNumInputFiles(); i++ ){
        if( !SCDeleteFile(Opts->GetInputFile(i)+".o") ){
          Msgs->PrintMsg( L_ERROR, "Failed to delete LLVM object file " +
                          Opts->GetInputFile(i) + ".o" );
          return false;
        }
      }
    }
  }

  return true;
}

// EOF
