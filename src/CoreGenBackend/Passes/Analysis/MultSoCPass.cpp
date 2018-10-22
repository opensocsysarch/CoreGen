//
// _MultSoCPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/MultSoCPass.h"

MultSoCPass::MultSoCPass(std::ostream *O,
                                     CoreGenDAG *D,
                                     CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"MultSoCPass",
                "Examines IR and warns if multiple SoC nodes are present",
                false,O,D,E) {
}

MultSoCPass::~MultSoCPass(){
}

bool MultSoCPass::Execute(){
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

  std::vector<CoreGenNode *> SoCNodes;

  // Walk all the nodes.  If we find multiple SoC nodes, flag a warning
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    if( D1->FindNodeByIndex(i)->GetType() == CGSoc ){
      SoCNodes.push_back(D1->FindNodeByIndex(i));
    }
  }

  if( SoCNodes.size() > 1 ){
    rtn = false;
    // print a global warning, then print all the SoC nodes
    WriteMsg( "Identified multiple SoC nodes.  Consider splitting into separate projects");
    for( unsigned i=0; i<SoCNodes.size(); i++ ){
      WriteMsg(" ==> SoCNode Index:Name = " + std::to_string(i) +
               ":" + SoCNodes[i]->GetName());
    }
  }

  return rtn;
}

// EOF
