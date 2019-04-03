//
// _CoreSafetyPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/CoreSafetyPass.h"

CoreSafetyPass::CoreSafetyPass(std::ostream *O,
                             CoreGenDAG *D,
                             CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,2,"CoreSafetyPass",
                "Checks various safety parameters for each defined core",
                false,O,D,E ){
}

CoreSafetyPass::~CoreSafetyPass(){
}

// Find cores that are not connected to SoC's or Plugins
bool CoreSafetyPass::FindDanglingCores(CoreGenDAG *DAG,
                                       CoreGenCore *Core,
                                       unsigned Idx ){

  unsigned sum = 0;
  for( unsigned i=0; i<DAG->GetDimSize(); i++ ){
    sum += DAG->AdjMat[i][Idx];
  }
  if( sum == 0 ){
    WriteMsg( "Identified a core not connected to the SoC; Core=" +
              Core->GetName() );
    return false;
  }
  return true;
}

bool CoreSafetyPass::Execute(){
  // Get the correct DAG level
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;

  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    CoreGenCore *CORE = static_cast<CoreGenCore *>(D1->FindNodeByIndex(i));
    if( CORE->GetType() == CGCore ){
      if( !FindDanglingCores(D1, CORE, i) ){
        rtn = false;
      }
    }
  }

  return rtn;
}

// EOF
