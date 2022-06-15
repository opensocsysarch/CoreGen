//
// _VLIWSafetypass_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/VLIWSafetyPass.h"

VLIWSafetyPass::VLIWSafetyPass(std::ostream *O,
                       CoreGenDAG *D,
                       CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"VLIWSafetyPass",
                "Identifies potential issues in VLIW pipeline stages",
                false,O,D,E) {
}

VLIWSafetyPass::~VLIWSafetyPass(){
}

bool VLIWSafetyPass::CheckInstFormat( CoreGenDAG *D ){
  bool rtn = true;
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    CoreGenVLIWStage *Stage = static_cast<CoreGenVLIWStage*>(D->FindNodeByIndex(i));
    if( Stage->GetType() == CGVLIW ){
      if( Stage->GetFormat() == nullptr ){
        WriteMsg( "VLIWStage = " + Stage->GetName() +
                  " does not have a defined instruction format" );
        rtn = false;
      }else if( Stage->GetFormat()->GetISA() != nullptr ){
        if( !Stage->GetFormat()->GetISA()->IsVLIW() ){
          WriteMsg("VLIWStage = " + Stage->GetName() +
                  " is associated with a non-VLIW instruction format" );
          rtn = false;
        }
      }
    }
  }
  return rtn;
}

bool VLIWSafetyPass::Execute(){
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

  // check the instruction format connectivity
  if( !CheckInstFormat(D1) ){
    rtn = false;
  }

  return rtn;
}

// EOF
