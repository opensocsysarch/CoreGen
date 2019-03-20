//
// _CacheLevelOpt_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Opt/CacheLevelOpt.h"

CacheLevelOpt::CacheLevelOpt(std::ostream *O,
                         CoreGenDAG *D,
                         CoreGenErrno *E)
  : CoreGenPass(CGPassOpt,1,"CacheLevelOpt",
                "Optimizes the layout cache hierarchies",
                false,O,D,E) {
}

CacheLevelOpt::~CacheLevelOpt(){
}

bool CacheLevelOpt::Execute(){
  // Get the correct DAG level: 1
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Lvel " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  // Walk all the nodes and flag the appropriate cache nodes
  // if the cache nodes have multiple parents (eg, they are shared child levels)
  // mark the appropriate attribute
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    if( D1->FindNodeByIndex(i)->GetType() == CGCache ){
      CoreGenCache *C = static_cast<CoreGenCache *>(D1->FindNodeByIndex(i));
      if( C->IsSubLevel() ){
        unsigned Parents = 0;
        for( unsigned j=0; j<D1->GetDimSize(); j++ ){
          if( D1->FindNodeByIndex(j)->GetType() == CGCache){
            // found an adjacent cache node
            Parents += D1->AdjMat[j][i];
          }
        }
        if( Parents > 1 ){
          C->SetAttr(AttrSharedCache);
        }
      }
    }
  }

  return true;
}

// EOF
