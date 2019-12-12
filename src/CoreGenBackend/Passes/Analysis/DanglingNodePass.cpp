//
// _DanglingNodePass_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/DanglingNodePass.h"

DanglingNodePass::DanglingNodePass(std::ostream *O,
                                   CoreGenDAG *D,
                                   CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1, "DanglingNodePass",
                "Identifies any potential dangling or unconnected nodes",
                false,O,D,E) {
}

DanglingNodePass::~DanglingNodePass(){
}

bool DanglingNodePass::Execute(){


  std::vector<unsigned> Idx;      // vector of suspect indices

  // Get the correct DAG Level 1
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }
  unsigned D = D1->GetDimSize(); // AdjMat dimensions

  // Initial part of the pass looks at each
  // source index and determines whether it
  // points to any dependent children.
  //
  // If not, it pushed onto an index vector
  // such that we can do a secondary search
  // We want to ignore the register nodes as these
  // do not have sources
  for( unsigned i=0; i<D; i++ ){
    unsigned Sum = 0;
    for( unsigned j=0; j<D; j++ ){
      Sum += D1->AdjMat[i][j];
    }
#if 0
    if( (Sum == 0) &&
        (D1->FindNodeByIndex(i)->GetType() != CGReg) ){
#endif
    if( Sum == 0 ){
      // not pointing to anything
      // add it to our index vector for a secondary test
      Idx.push_back(i);
#if 0
      WriteMsg( "No source from node at DAG node Index:Name = {" +
                std::to_string(i) + ":" +
                D1->FindNodeByIndex(i)->GetName() + "}");
#endif
    }
  }

  bool rtn = true;  // signals success/failure of the pass

  // Secondary search
  // Search the i'th vertical vector for any positive values
  for( unsigned i=0; i<Idx.size(); i++ ){
    unsigned Sum = 0;
    for( unsigned j=0; j<D; j++ ){
      Sum += D1->AdjMat[j][Idx[i]];
    }
    if( Sum == 0 ){
      // nothing pointing to this index either
      // return a failed pass and print a message
      rtn = false;
      WriteMsg( "Identified a dangling node at DAG node Index:Name = {" +
                std::to_string(Idx[i]) + ":" +
                D1->FindNodeByIndex(Idx[i])->GetName() + "}");
    }
  }

  return rtn;
}

// EOF
