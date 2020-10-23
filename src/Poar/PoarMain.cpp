//
// _PoarMain_cpp__
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>

#include "CoreGen/Poar/PoarOpts.h"
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/StoneCutter/StoneCutter.h"

int main( int argc, char **argv ){
  // parse the command line options
  // read in any configuration templates
  // read in coregen ir
  // if no SC specified, derived sc from coregen ir
  // else, read in sc
  // derive report
  // print it

  // -- classes --
  // options handler
  // configuration template handler
  // data handler
  // printer

  PoarOpts *POpts = new PoarOpts(argc,argv);

  bool isHelp = false;
  bool res = POpts->ParseOpts(&isHelp);
  if( isHelp ){
    delete POpts;
    return 0;
  }else if( !res ){
    // encountered an error
    delete POpts;
    return -1;
  }

  delete POpts;

  return 0;
}
