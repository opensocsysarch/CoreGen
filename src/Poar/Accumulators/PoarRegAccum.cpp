//
// _PoarRegAccum_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarRegAccum.h"

bool PoarRegAccum::Accum(){
  // flag this accumulator as being executed
  this->FlagRun();

  // Stage 1: Find all the cores
  std::vector<CoreGenNode *> Cores;
  if( AccumNodes(Cores,CGCore) ){
    return false;
  }

  // Stage 2: Find all the register classes
  std::vector<CoreGenNode *> RegClasses;
  if( AccumNodes(RegClasses,CGRegC) ){
    return false;
  }

  return true;
}

// EOF
