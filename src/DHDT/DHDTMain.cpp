//
// _DHDTMain_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>
#include "CoreGen/DHDT/DHDTOpts.h"
#include "CoreGen/DHDT/DHDTGraph.h"

int main( int argc, char **argv ){
  // parse the command line opts
  DHDTOpts *Opts = new DHDTOpts(argc, argv);
  if( !Opts ){
    std::cout << "Error : could not load DHDT options" << std::endl;
    return -1;
  }

  bool isHelp = false;
  bool res = Opts->ParseOpts(isHelp);
  if( isHelp ){
    delete Opts;
    return 0;
  }else if( !res ){
    delete Opts;
    return -1;
  }

  // create the graph structure
  DHDTGraph Graph;

  if( !Graph.ReadLLVMIRGraph(Opts->GetLLVMIRFile()) ){
    delete Opts;
    return -1;
  }

  delete Opts;
  return 0;
}

// EOF
