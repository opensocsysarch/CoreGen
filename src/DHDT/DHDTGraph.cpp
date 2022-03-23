//
// _DHDTGraph_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/DHDT/DHDTGraph.h"

DHDTGraph::DHDTGraph(){}

DHDTGraph::~DHDTGraph(){}

bool DHDTGraph::ReadLLVMIRGraph(std::string IR){
  if( IR.length() == 0 ){
    return false;
  }

  Mod = parseIRFile(IR.c_str(), Err, Context);
  if( !Mod ){
    std::cout << "Error parsing IR File: " << IR << std::endl;
    Err.print(IR.c_str(), errs());
    return false;
  }

  return true;
}

// EOF
