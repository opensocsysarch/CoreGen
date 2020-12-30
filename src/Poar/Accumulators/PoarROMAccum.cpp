//
// _PoarROMAccum_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarROMAccum.h"

bool PoarROMAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // We currently have no ability to derive this as we don't have a node
  // to define ROM's

  return true;
}

// EOF
