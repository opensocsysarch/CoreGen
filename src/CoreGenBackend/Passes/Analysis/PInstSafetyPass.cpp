//
// _PInstSafetyPass_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/PInstSafetyPass.h"

PInstSafetyPass::PInstSafetyPass(std::ostream *O,
                           CoreGenDAG *D,
                           CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"PInstSafetyPass",
                "Identifies issues in using Pseudo Instructions",
                false,O,D,E) {
}

PInstSafetyPass::~PInstSafetyPass(){
}

bool PInstSafetyPass::CheckISACompat( CoreGenDAG *D ){
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    CoreGenPseudoInst *PI = static_cast<CoreGenPseudoInst*>(D->FindNodeByIndex(i));
    if( PI->GetType() == CGPInst ){
      // found a pseudo instruction node
      CoreGenISA *ISA = PI->GetISA();

      // make sure the ISA is valid
      if( ISA == NULL ){
        WriteMsg( "Identified a pseudo instruction with no associated ISA; PseudoInst=" +
                  PI->GetName() );
        return false;
      }else if( PI->GetInst() == NULL ){
        WriteMsg( "Identified a pseudo instruction with no root instruction; PseudoInst=" +
                  PI->GetName() );
        return false;
      }else if( PI->GetInst()->GetISA() == NULL ){
        WriteMsg( "Identified a pseudo instruction whose root instruction has no ISA; PseudoInst=" +
                  PI->GetName() + "; Inst=" + PI->GetInst()->GetName() );
        return false;
      }

      // make sure the pseudo inst and inst ISA's match
      if( ISA != PI->GetInst()->GetISA() ){
        WriteMsg( "Identified a mismatch in pseudo instruction ISA and instruction ISA; PseudoInst=" +
                  PI->GetName() + "; PInst ISA=" + ISA->GetName() + "; Inst ISA=" + 
                  PI->GetInst()->GetISA()->GetName() );
        return false;
      }
    }
  }
  return true;
}

bool PInstSafetyPass::Execute(){
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
  if( !CheckISACompat(D1) ){
    rtn = false;
  }

  return rtn;
}
