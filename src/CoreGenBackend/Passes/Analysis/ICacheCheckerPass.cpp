//
// _ICacheCheckerPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/ICacheCheckerPass.h"

ICacheCheckerPass::ICacheCheckerPass(std::ostream *O,
                                     CoreGenDAG *D,
                                     CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"ICacheCheckerPass",
                "Identifies any Cores that potentially lack an ICache",
                false,O,D,E) {
}

ICacheCheckerPass::~ICacheCheckerPass(){
}

bool ICacheCheckerPass::Execute(){
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

  // Walk through all the source nodes and determine if we find a core
  // If the node is a core, walk all its children and look for a cache
  // node.  If no cache nodes are found, then flag an alert
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    if( D1->FindNodeByIndex(i)->GetType() == CGCore ){
      bool found = false;
      for( unsigned j=0; j<D1->GetDimSize(); j++ ){
        if( (D1->AdjMat[i][j] == 1) &&
            (D1->FindNodeByIndex(j)->GetType() == CGCache) )
          found = true;
      }

      // check the state of the core and flag an issue
      if( !found ){
        rtn = false;
        WriteMsg( "Warning: found a core that has no instruction cache; may cause performance issues; Core=" +
                  D1->FindNodeByIndex(i)->GetName() );
      }
    }
  }

  return rtn;
}

// EOF
