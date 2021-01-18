//
// _PoarCPathAccum_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarCPathAccum.h"

bool PoarCPathAccum::SearchMemoryCtrlPath(){

  std::vector<CoreGenNode *> Mems;
  if( !AccumNodes(Mems,CGMCtrl) ){
    return false;
  }

  for( unsigned i=0; i<Mems.size(); i++ ){
    // For each memory controller, we have control bits for:
    // 1 byte, 2 byte 4 byte, 8 byte operations [4 bits]
    // read operations [1 bit]
    // write operations [1 bit]
    // back pressure [1 bit]
    // clock [1 bit]
    AddWidth( (uint64_t)(8) );
  }

  return true;
}

bool PoarCPathAccum::SearchInstructionCtrlPath(){

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

  // For each core, compile the set of instruction encoding fields
  for( unsigned i=0; i<Cores.size(); i++ ){
    CoreGenCore *C = static_cast<CoreGenCore *>(Cores[i]);
    CoreGenISA *I = C->GetISA();

    // determine if we have an instruction format in range
    // accumulate all the control paths for it
    for( unsigned j=0; j<Formats.size(); j++ ){
      CoreGenInstFormat *F = static_cast<CoreGenInstFormat *>(Formats[j]);
      if( F->GetISA() == I ){
        // in range
        for( unsigned k=0; k<F->GetNumFields(); k++ ){
          if( F->GetFieldType(F->GetFieldName(k)) == CoreGenInstFormat::CGInstCode ){
            AddWidth( (uint64_t)(F->GetFieldWidth(F->GetFieldName(k))) );
          }
        }
      }
    }

  }

  return true;
}

bool PoarCPathAccum::SearchRegisterCtrlPath(){

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

        // we had a control bit for every register (hazard)
        // as well as enable bits for the read and write ports
        AddWidth( (uint64_t)(RC->GetNumReg()) +
                  (uint64_t)(RC->GetReadPorts()) +
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

        // we had a control bit for every register (hazard)
        // as well as enable bits for the read and write ports
        AddWidth( (uint64_t)(RC->GetNumReg()) +
                  (uint64_t)(RC->GetReadPorts()) +
                  (uint64_t)(RC->GetWritePorts()) );
    }
  }

  return true;
}

bool PoarCPathAccum::SearchCoreToCache(){

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
      // add 4 bits of control (mshr)
      AddWidth( (uint64_t)(4) );
    }
  }

  return true;
}

bool PoarCPathAccum::SearchCacheLevels(){

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

  // add four bits of control for each cache sublevel.  we need to count every connection
  // (not just the unique ones) since the control paths will fan out
  for( unsigned i=0; i<SubLevels.size(); i++ ){
    AddWidth( (uint64_t)(4) );
  }

  return true;
}

bool PoarCPathAccum::SearchCacheCtrlPath(){

  // search from core to cache
  if( !SearchCoreToCache() )
    return false;

  // search within the cache hierarchy
  if( !SearchCacheLevels() )
    return false;

  return true;
}

bool PoarCPathAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: Derive the memory control path parameters
  if( !SearchMemoryCtrlPath() )
    return false;

  // Stage 2: Derive the instruction control path parameters
  if( !SearchInstructionCtrlPath() )
    return false;

  // Stage 3: Derive the register control path parameters
  if( !SearchRegisterCtrlPath() )
    return false;

  // Stage 4: Derive the cache control path parameters
  if( !SearchCacheCtrlPath() )
    return false;

  return true;
}

// EOF
