//
// _PoarDPathAccum_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarDPathAccum.h"

bool PoarDPathAccum::SearchMemoryDataPath(){

  std::vector<CoreGenNode *> Mems;
  if( !AccumNodes(Mems,CGMCtrl) ){
    return false;
  }

  for( unsigned i=0; i<Mems.size(); i++ ){
    CoreGenMCtrl *M = static_cast<CoreGenMCtrl *>(Mems[i]);

    AddWidth( (uint64_t)(M->GetNumInputPorts() * 64) );
  }

  return true;
}

bool PoarDPathAccum::SearchInstructionDataPath(){

  // Pull all the cores
  std::vector<CoreGenNode *> Cores;
  if( !AccumNodes(Cores,CGCore) ){
    return false;
  }

  // Pull all the inst formats
  std::vector<CoreGenNode *> Formats;
  if( !AccumNodes(Formats,CGInstF) ){
    return false;
  }

  // For each core, derive the largest instruction width
  for( unsigned i=0; i<Cores.size(); i++ ){
    CoreGenCore *C = static_cast<CoreGenCore *>(Cores[i]);
    CoreGenISA *I = C->GetISA();
    uint64_t max = 0x00ull;

    // determine if we have an instruction format in range
    // if so, find the maximum instruction format for the target core
    for( unsigned j=0; j<Formats.size(); j++ ){
      CoreGenInstFormat *F = static_cast<CoreGenInstFormat *>(Formats[j]);
      if( F->GetISA() == I ){
        if( (uint64_t)(F->GetFormatWidth()) > max ){
          max = (uint64_t)(F->GetFormatWidth());
        }
      }
    }

    AddWidth(max);
  }

  return true;
}

bool PoarDPathAccum::SearchRegisterDataPath(){

  // Pull all the cores
  std::vector<CoreGenNode *> Cores;
  if( !AccumNodes(Cores,CGCore) ){
    return false;
  }

  if( Cores.size() > 0 ){
    // accumulate only what is connected to the cores
    // all others will be optimized out
    for( unsigned i=0; i<Cores.size(); i++ ){
      CoreGenCore *C = static_cast<CoreGenCore *>(Cores[i]);

      for( unsigned j=0; j<C->GetNumRegClass(); j++ ){
        CoreGenRegClass *RC = C->GetRegClass(j);
        uint64_t MaxWidth = (uint64_t)(RC->GetMaxWidth());
        AddWidth( MaxWidth *
                  (uint64_t)(RC->GetReadPorts()) *
                  (uint64_t)(RC->GetWritePorts()) );
      }
    }
  }else{
    // accumulate the register classes that we can find
    std::vector<CoreGenNode *> RegClasses;
    if( !AccumNodes(RegClasses,CGRegC) ){
      return false;
    }

    for( unsigned i=0; i<RegClasses.size(); i++ ){
      CoreGenRegClass *RC = static_cast<CoreGenRegClass *>(RegClasses[i]);
      uint64_t MaxWidth = (uint64_t)(RC->GetMaxWidth());
      AddWidth( MaxWidth *
                (uint64_t)(RC->GetReadPorts()) *
                (uint64_t)(RC->GetWritePorts()) );
    }
  }

  return true;
}

bool PoarDPathAccum::SearchCoreToCache(){

  // pull all the cores
  std::vector<CoreGenNode *> Cores;
  if( !AccumNodes(Cores,CGCore) ){
    return false;
  }

  // look for the datapath between each core and primary cache level
  for( unsigned i=0; i<Cores.size(); i++ ){
    CoreGenCore *C = static_cast<CoreGenCore *>(Cores[i]);
    CoreGenCache *Cache = C->GetCache();

    if( Cache ){
      // we found a valid cache, the data path will be
      // equivalent to the largest register found
      unsigned Max = 0;
      for( unsigned j=0; j<C->GetNumRegClass(); j++ ){
        if( C->GetRegClass(j)->GetMaxWidth() > Max )
          Max = C->GetRegClass(j)->GetMaxWidth();
      }
      AddWidth(Max);
    }
  }

  return true;
}

bool PoarDPathAccum::SearchCacheLevels(){

  // pull all the cache nodes
  std::vector<CoreGenNode *> Caches;
  if( !AccumNodes(Caches,CGCache) ){
    return false;
  }

  // walk through and identify the cache sublevels
  std::vector<CoreGenCache *> SubLevels;
  for( unsigned i=0; i<Caches.size(); i++ ){
    CoreGenCache *C = static_cast<CoreGenCache *>(Caches[i]);

    if( C->IsParentLevel() && !C->IsSubLevel() ){
      AddWidth( (uint64_t)(C->GetLineSize() * 8) );
      if( C->GetSubCache() != nullptr )
        SubLevels.push_back(C->GetSubCache());
    }else{
      SubLevels.push_back(C);
    }
  }

  // make the sub-levels unique
  std::sort(SubLevels.begin(), SubLevels.end());
  SubLevels.erase( std::unique( SubLevels.begin(), SubLevels.end() ), SubLevels.end() );

  for( unsigned i=0; i<SubLevels.size(); i++ ){
    AddWidth( SubLevels[i]->GetLineSize() * 8 * SubLevels[i]->GetNumParentCache() );
  }

  return true;
}

bool PoarDPathAccum::SearchCacheDataPath(){

  // search from core to cache
  if( !SearchCoreToCache() )
    return false;

  // search within the cache hierarchy
  if( !SearchCacheLevels() )
    return false;

  return true;
}

bool PoarDPathAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: Derive the memory data path parameters
  if( !SearchMemoryDataPath() )
    return false;

  // Stage 2: Derive the instruction data path parameters
  if( !SearchInstructionDataPath() )
    return false;

  // Stage 3: Derive the register data path parameters
  if( !SearchRegisterDataPath() )
    return false;

  // Stage 4: Derive the cache data path parameters
  if( !SearchCacheDataPath() )
    return false;

  return true;
}

// EOF
