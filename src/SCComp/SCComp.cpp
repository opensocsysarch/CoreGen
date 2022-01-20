//
// _SCComp_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//


#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "CoreGen/StoneCutter/StoneCutter.h"

// ------------------------------------------------- MAIN
int main( int argc, char **argv ){

  SCMsg *Msgs = new SCMsg();
  SCOpts *Opts = new SCOpts(Msgs,argc, argv);
  int fini = 0;

  // parse the incoming cli opts
  bool isHelp = false;
  bool res = Opts->ParseOpts(&isHelp);
  if( isHelp ){
    // printed the help menu
    delete Opts;
    delete Msgs;
    return 0;
  }else if( !res ){
    // encountered an error
    delete Opts;
    delete Msgs;
    return -1;
  }

  // execute the compiler
  SCExec *ExecCtx = new SCExec(Opts,Msgs);

  if( !ExecCtx->Exec() ){
    fini = -1;
  }

  // clean up
  delete ExecCtx;
  delete Opts;
  delete Msgs;
  return fini;
}

// EOF
