//
// _CacheLevelPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/CacheLevelPass.h"

CacheLevelPass::CacheLevelPass(std::ostream *O,
                                     CoreGenDAG *D,
                                     CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"CacheLevelPass",
                "Tests cache hierarchy to ensure child levels are correctly connected",
                false,O,D,E) {
}

CacheLevelPass::~CacheLevelPass(){
}

bool CacheLevelPass::Execute(){
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

  // walk all the nodes.  if we find a cache node with parents or children,
  // ensure they're connected
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    CoreGenCache *C = static_cast<CoreGenCache *>(D1->FindNodeByIndex(i));
    if( C->GetType() == CGCache ){
      // check my parent status
      if( (C->IsSubLevel()) && (C->GetParentCache() == nullptr) ){
        WriteMsg( "Identified a child cache level with no parent at Index:Name = " +
                std::to_string(i) + ":" + C->GetName());
        rtn = false;
      }

      // check my child status
      if( (C->IsParentLevel()) && (C->GetSubCache() == nullptr ) ){
        WriteMsg( "Identified a parent cache level with no child and the sublevel flag is set at Index:Name = " +
                std::to_string(i) + ":" + C->GetName());
        rtn = false;
      }
    }
  }

  return rtn;
}

// EOF
