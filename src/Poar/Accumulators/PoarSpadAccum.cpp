//
// _PoarSpadAccum_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarSpadAccum.h"

bool PoarSpadAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: Find all the scratchpads
  std::vector<CoreGenNode *> Spads;
  if( !AccumNodes(Spads,CGSpad) ){
    return false;
  }

  // Stage 2: Add all the spad bits
  for( unsigned i=0; i<Spads.size(); i++ ){
    AddWidth( (uint64_t)(static_cast<CoreGenSpad *>(Spads[i])->GetMemSize())*8 );
  }

  return true;
}

// EOF
