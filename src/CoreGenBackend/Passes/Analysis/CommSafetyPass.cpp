//
// _CommSafetyPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/CommSafetyPass.h"

CommSafetyPass::CommSafetyPass(std::ostream *O,
                           CoreGenDAG *D,
                           CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,2,"CommSafetyPass",
                "Identifies issues in using Comm module connectivity",
                false,O,D,E) {
}

CommSafetyPass::~CommSafetyPass(){
}

bool CommSafetyPass::CheckCommCompatP2P(CoreGenDAG *D, unsigned Root){
  unsigned Sum = 0;
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum += D->AdjMat[Root][i];
  }
  if( Sum > 2 ){
    WriteMsg( "Comm channel = " + D->FindNodeByIndex(Root)->GetName() +
              " has too many connections for P2P configuration" );
    return false;
  }

  Sum = 0;
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum += D->AdjMat[i][Root];
  }
  if( Sum > 2 ){
    WriteMsg( "Comm channel = " + D->FindNodeByIndex(Root)->GetName() +
              " has too many connections for P2P configuration" );
    return false;
  }
  return true;
}

bool CommSafetyPass::CheckCommCompatBus(CoreGenDAG *D, unsigned Root){
  unsigned Sum1 = 0;
  unsigned Sum2 = 0;
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum1 += D->AdjMat[Root][i];
  }
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum2 += D->AdjMat[i][Root];
  }
  if( (Sum1==0) && (Sum2==0) ){
    WriteMsg( "Comm channel = " + D->FindNodeByIndex(Root)->GetName() +
              " has insufficient connections for Bus configuration" );
    return false;
  }
  return true;
}

bool CommSafetyPass::CheckCommCompatNOC(CoreGenDAG *D, unsigned Root){
  unsigned Sum1 = 0;
  unsigned Sum2 = 0;
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum1 += D->AdjMat[Root][i];
  }
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum2 += D->AdjMat[i][Root];
  }
  if( (Sum1==0) && (Sum2==0) ){
    WriteMsg( "Comm channel = " + D->FindNodeByIndex(Root)->GetName() +
              " has insufficient connections for NOC configuration" );
    return false;
  }
  return true;
}

bool CommSafetyPass::CheckCommCompatUnk(CoreGenDAG *D, unsigned Root){
  unsigned Sum1 = 0;
  unsigned Sum2 = 0;
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum1 += D->AdjMat[Root][i];
  }
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    Sum2 += D->AdjMat[i][Root];
  }
  if( (Sum1==0) && (Sum2==0) ){
    WriteMsg( "Comm channel = " + D->FindNodeByIndex(Root)->GetName() +
              " has insufficient connections for Unknown configuration" );
    return false;
  }
  return true;
}

bool CommSafetyPass::CheckCommCompat( CoreGenDAG *D ){
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    CoreGenComm *Comm = static_cast<CoreGenComm*>(D->FindNodeByIndex(i));
    if( Comm->GetType() == CGComm ){
      // found a comm node
      // look at the comm type
      switch( Comm->GetCommType() ){
      case CGCommP2P:
        if( !CheckCommCompatP2P(D,i) ){
          return false;
        }
        break;
      case CGCommBus:
        if( !CheckCommCompatBus(D,i) ){
          return false;
        }
        break;
      case CGCommNoc:
        if( !CheckCommCompatNOC(D,i) ){
          return false;
        }
        break;
      case CGCommUnk:
      default:
        if( !CheckCommCompatUnk(D,i) ){
          return false;
        }
        break;
      }
    }
  }
  return true;
}

bool CommSafetyPass::Execute(){
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

  // check the isa compatability
  if( !CheckCommCompat(D1) ){
    rtn = false;
  }

  return rtn;
}
