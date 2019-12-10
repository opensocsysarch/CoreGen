//
// _SafeDeletePass_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/SafeDeletePass.h"

SafeDeletePass::SafeDeletePass(std::ostream *O,
                       CoreGenDAG *D,
                       CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,3,"SafeDeletePass",
                "Determines whether a node can be safely deleted",
                true, O,D,E) {
}

SafeDeletePass::~SafeDeletePass(){
}

bool SafeDeletePass::Execute(){
  // Get the correct DAG level: 3
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D3 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  if( this->GetTargetNode() == nullptr ){
    WriteMsg( "Error: no target node defined" );
    Errno->SetError( CGERR_STATUS, this->GetName() +
                     " : No target node defined" );
    return false;
  }

  bool rtn = true;
  unsigned TargetIdx = D3->FindIndexByNode( this->GetTargetNode() );
  std::vector<unsigned> SourceIdxs;
  std::vector<unsigned> DestIdxs;

  // walk all the nodes and examine the intersection
  // of the target node and any potential adjacent nodes
  for(unsigned i=0; i<D3->GetDimSize(); i++ ){
    if( D3->AdjMat[TargetIdx][i] == 1 ){
      SourceIdxs.push_back(i);
    }
    if( D3->AdjMat[i][TargetIdx] == 1 ){
      DestIdxs.push_back(i);
    }
  }

  std::string SourceString;
  std::string DestString;
  bool warning = false;

  // tally up any discrepancies
  if( SourceIdxs.size() > 0 ){
    warning = true;
    for( unsigned i=0; i<SourceIdxs.size(); i++ ){
      SourceString += " : " + D3->FindNodeByIndex(SourceIdxs[i])->GetName();
    }
  }
  if( DestIdxs.size() > 0 ){
    rtn = false;
    for( unsigned i=0; i<DestIdxs.size(); i++ ){
      DestString += " : " + D3->FindNodeByIndex(DestIdxs[i])->GetName();
    }
  }

  if( warning ){
    WriteMsg( "TargetNode : " +
              this->GetTargetNode()->GetName() +
              " will be removed as children from " +
              DestString);
  }

  if( !rtn ){
    WriteMsg( "TargetNode : " +
              this->GetTargetNode()->GetName() +
              " will break dependent links from the following nodes " +
              DestString);
    Errno->SetError( CGERR_STATUS, this->GetName() +
                     "TargetNode : " + this->GetTargetNode()->GetName() +
                     " will break dependent links from the following nodes " +
                     DestString);
  }

  return rtn;
}

// EOF
