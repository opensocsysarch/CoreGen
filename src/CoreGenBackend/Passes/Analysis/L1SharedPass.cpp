//
// _L1SharedPass_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/L1SharedPass.h"

L1SharedPass::L1SharedPass(std::ostream *O,
                                     CoreGenDAG *D,
                                     CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"L1SharedPass",
                "Identifies any potential L1 Caches that are potentially shared across Cores",
                false,O,D,E) {
}

L1SharedPass::~L1SharedPass(){
}

bool L1SharedPass::Execute(){
  // Get the correct DAG level: 1
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;

  std::map<CoreGenNode *,unsigned> CacheMap;

  // Walk through all the source nodes and determine if we find a core
  // If the node is a core, walk all its children and look for a cache
  // node.  If the cache node is an L1 node (no parent levels), then
  // flag it for further analysis.  If the respective node has more than
  // one associated core, flag warning
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    if( D1->FindNodeByIndex(i)->GetType() == CGCore ){
      for( unsigned j=0; j<D1->GetDimSize(); j++ ){
        if( (D1->AdjMat[i][j] == 1) &&
            (D1->FindNodeByIndex(j)->GetType() == CGCache) ){
          // Cache node on the i'th core.  Retrieve it and
          // check to see if it has any parents
          CoreGenCache *C = static_cast<CoreGenCache *>(D1->FindNodeByIndex(j));
          if( !C->IsSubLevel() ){
            // Identified a parent cache node
            auto Search = CacheMap.find(C);
            if( Search == CacheMap.end() ){
              // insert it
              CacheMap.insert(std::pair<CoreGenNode *,unsigned>(C,1));
            }else{
              // nudge the counter
              Search->second += 1;
            }
          }
        }
      }
    }
  }

  // Stage 2: Analyze all the found nodes
  for( auto const& it : CacheMap ){
    if( it.second > 1 ){
      // we found an L1 cache node shared amongst multiple cores
      rtn = false;
      WriteMsg( "Warning: found an L1 cache node shared amongst multiple cores; may cause unnecessary coherency traffic; Cache=" + it.first->GetName() );
    }
  }

  return rtn;
}

// EOF
