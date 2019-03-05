//
// _MandatoryFieldPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/MandatoryFieldPass.h"

MandatoryFieldPass::MandatoryFieldPass(std::ostream *O,
                                       CoreGenDAG *D,
                                       CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,3,"MandatoryFieldPass",
                "Identifies discrepancies in the number of mandatory fields defined and the number of defined fields",
               false,O,D,E){
}

MandatoryFieldPass::~MandatoryFieldPass(){
}

bool MandatoryFieldPass::Execute(){
  // Get the correct DAG level: 3
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D3 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;

  // Walk the DAG nodes and identify unique instruction nodes
  // For each instruction node, retrieve the number of mandatory
  // fields defined.  Check the defined fields against the mandatory
  // set.
  for( unsigned i=0; i<D3->GetDimSize(); i++ ){
    if( D3->FindNodeByIndex(i)->GetType() == CGInst ){
      CoreGenInst *Inst = static_cast<CoreGenInst *>(D3->FindNodeByIndex(i));
      CoreGenInstFormat *Format = Inst->GetFormat();
      if( Format ){
        // walk all the encoding bits and match the mandatory fields
        for( unsigned j=0; j<Format->GetNumFields(); j++ ){
          if( Format->GetMandatoryFlag( Format->GetFieldName(j) ) ){
            // this field is mandatory
            bool Success = true;
            Inst->GetEncoding( Format->GetFieldName(j),
                               &Success );
            if( !Success ){
              WriteMsg( "Mandatory field: " + Format->GetFieldName(j) +
                        " not defined for instruction: " + Inst->GetName() );
              rtn = false;
            }
          }
        }
      }else{
        // no encoding defined
        WriteMsg( "No format defined for instruction: " + Inst->GetName() );
        rtn = false;
      }

    }
  }

  return rtn;
}

// EOF
