//
// _PoarCacheAccum_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarCacheAccum.h"

bool PoarCacheAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: Find all the scratchpads
  std::vector<CoreGenNode *> Caches;
  if( !AccumNodes(Caches,CGCache) ){
    return false;
  }

  uint64_t Sets = 0x00ull;
  uint64_t Ways = 0x00ull;
  uint64_t LSize = 0x00ull;

  // Stage 2: Add all the cache bits
  for( unsigned i=0; i<Caches.size(); i++ ){
    CoreGenCache *C = static_cast<CoreGenCache *>(Caches[i]);
    Sets = (uint64_t)(C->GetSets());
    Ways = (uint64_t)(C->GetWays());
    LSize = (uint64_t)(C->GetLineSize());
    AddWidth(Sets*Ways*LSize*8);
  }

  return true;
}

// EOF
