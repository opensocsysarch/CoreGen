//
// _RegClassOpt_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Opt/RegClassOpt.h"

RegClassOpt::RegClassOpt(std::ostream *O,
                         CoreGenDAG *D,
                         CoreGenErrno *E)
  : CoreGenPass(CGPassOpt,1,"RegClassOpt",
                "Optimizes the layout of register files",
                false,O,D,E) {
}

RegClassOpt::~RegClassOpt(){
}

bool RegClassOpt::Execute(){
  return true;
}

// EOF
