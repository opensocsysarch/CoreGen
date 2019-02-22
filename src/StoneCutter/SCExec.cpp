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

  Parser = new SCParser(Buf,TmpFile,Opts,Msgs);
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

  std::string LTmpFile; //tmp input
  std::string OTmpFile; //tmp output
  if( Opts->GetNumInputFiles() > 1 ){
    // retrieve all the files and consolidate them into a single input file
    char *tmpname = strdup("/tmp/SCCTmpFileXXXXXX");
    mkstemp(tmpname);
    std::ofstream of(tmpname);
    for( unsigned i=0; i<Opts->GetNumInputFiles(); i++ ){
      std::ifstream infile( Opts->GetInputFile(i) );
      of << infile.rdbuf();
      infile.close();
    }
    of.close();
    LTmpFile = std::string(tmpname);
    OTmpFile = Opts->GetOutputFile();
    if( OTmpFile.length() == 0 ){
      OTmpFile = "scc.out";
    }
  }else{
    // single file
    LTmpFile = Opts->GetInputFile(0);
    OTmpFile = LTmpFile;
  }

  // Read the file into a buffer
  std::ifstream t(LTmpFile);
  std::string Buf( (std::istreambuf_iterator<char>(t)),
                     (std::istreambuf_iterator<char>()));

  Parser = new SCParser(Buf,LTmpFile,Opts,Msgs);
  if( !Parser ){
    Msgs->PrintMsg( L_ERROR, "Failed to initiate the StoneCutter parser" );
    SCDeleteFile(LTmpFile);
    return false;
  }

  // Do we execute the inline optimizer?
  if( Opts->IsOptimize() ){
    // first enable all the passes
    Parser->EnableAllPasses();

    // do we need to manually enable passes?
    if( Opts->IsEnablePass() ){
      if( !Parser->EnablePasses(Opts->GetEnablePass()) ){
        Msgs->PrintMsg( L_ERROR, "Failed to manually enable optimization passes" );
        delete Parser;
        if( Opts->GetNumInputFiles() > 1 ){
          SCDeleteFile(LTmpFile);
        }
        return false;
      }
    }

    // do we need to manually disable passes?
    if( Opts->IsDisablePass() ){
      if( !Parser->DisablePasses(Opts->GetDisabledPass()) ){
        Msgs->PrintMsg( L_ERROR, "Failed to manually disable optimization passes" );
        delete Parser;
        if( Opts->GetNumInputFiles() > 1 ){
          SCDeleteFile(LTmpFile);
        }
        return false;
      }
    }

    // enable the optimizer
    if( !Parser->Optimize() ){
      Msgs->PrintMsg( L_ERROR, "Failed to initialize the optimizer" );
      delete Parser;
      if( Opts->GetNumInputFiles() > 1 ){
        SCDeleteFile(LTmpFile);
      }
      return false;
    }
  }

  // We always want to execute the parser, if not, we fail
  // this may change in the future where we can read in
  // IR for code generation
  if( Opts->IsParse() ){
    // execute the parser
    if( !Parser->Parse() ){
      Msgs->PrintMsg( L_ERROR, "Failed to parse " + LTmpFile );
      delete Parser;
      if( Opts->GetNumInputFiles() > 1 ){
        SCDeleteFile(LTmpFile);
      }
      return false;
    }
  }else{
    // we have failed
    delete Parser;
    if( Opts->GetNumInputFiles() > 1 ){
      SCDeleteFile(LTmpFile);
    }
    return false;
  }

  SCLLCodeGen *CG = nullptr;

  // Do we execute the LLVM IR codegen?
  if( Opts->IsIR() ){
    // prep a new output file name; append '.ll'
    CG = new SCLLCodeGen(Parser,Msgs,
                         OTmpFile + ".ll",
                         OTmpFile + ".o");
    if( !CG->GenerateLL() ){
      Msgs->PrintMsg( L_ERROR, "Failed to generate IR for " +
                      LTmpFile );
      delete CG;
      delete Parser;
      if( Opts->GetNumInputFiles() > 1 ){
        SCDeleteFile(LTmpFile);
      }
      return false;
    }
  }

  SCChiselCodeGen *CCG = nullptr;

  // Do we generate Chisel?
  if( Opts->IsChisel() ){
    if( !Opts->IsIR() ){
      Msgs->PrintMsg( L_ERROR, "LLVM IR is required for Chisel output" );
      delete CG;
      delete Parser;
      if( Opts->GetNumInputFiles() > 1 ){
        SCDeleteFile(LTmpFile);
      }
      return false;
    }

    // Generate the Chisel output
    CCG = new SCChiselCodeGen(Parser,Opts,Msgs,
                              OTmpFile + ".chisel" );
    if( !CCG->GenerateChisel() ){
      Msgs->PrintMsg( L_ERROR, "Failed to generate Chisel for " +
                      LTmpFile );
      delete CCG;
      if( CG ){
        delete CG;
      }
      delete Parser;
      if( Opts->GetNumInputFiles() > 1 ){
        SCDeleteFile(LTmpFile);
      }
      return false;
    }
  }

    // Do we execute the object codegen
  if( Opts->IsCG() ){
    if( !Opts->IsIR() ){
      Msgs->PrintMsg( L_ERROR, "LLVM IR is required for object files" );
      delete CG;
      delete Parser;
      if( Opts->GetNumInputFiles() > 1 ){
        SCDeleteFile(LTmpFile);
      }
      return false;
    }

    if( !CG->GenerateObjFile() ){
      Msgs->PrintMsg( L_ERROR, "Failed to generate object file for " +
                      LTmpFile );
      if( CCG ){
        delete CCG;
      }
      if( CG ){
        delete CG;
      }
      delete Parser;
      if( Opts->GetNumInputFiles() > 1 ){
        SCDeleteFile(LTmpFile);
      }
      return false;
    }
  }

  if( CG ){
    delete CG;
  }
  if( CCG ){
    delete CCG;
  }
  delete Parser;


  // Do we toss the intermediate files?
  if( !Opts->IsKeep() ){
    // remove all the LL files
    if( Opts->IsIR() ){
      if( !SCDeleteFile(OTmpFile+".ll") ){
        Msgs->PrintMsg( L_ERROR, "Failed to delete LLVM IR file " +
                        OTmpFile + ".ll" );
        if( Opts->GetNumInputFiles() > 1 ){
          SCDeleteFile(LTmpFile);
        }
        return false;
      }
    }
    // remove all the object files
    if( Opts->IsCG() ){
      if( !SCDeleteFile(OTmpFile+".o") ){
        Msgs->PrintMsg( L_ERROR, "Failed to delete LLVM object file " +
                        OTmpFile + ".o" );
        if( Opts->GetNumInputFiles() > 1 ){
          SCDeleteFile(LTmpFile);
        }
        return false;
      }
    }
  }

  if( Opts->GetNumInputFiles() > 1 ){
    SCDeleteFile(LTmpFile);
  }
  return true;
}

// EOF
