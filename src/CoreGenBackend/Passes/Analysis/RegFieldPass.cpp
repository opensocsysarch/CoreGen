//
// _RegFieldPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/RegFieldPass.h"

RegFieldPass::RegFieldPass(std::ostream *O,
                           CoreGenDAG *D,
                           CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"RegFieldPass",
                "Identifies instruction encodings that require registers without a Register Class",
                false,O,D,E) {
}

RegFieldPass::~RegFieldPass(){
}

bool RegFieldPass::Execute(){
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

  // walk all the nodes and look for instruction formats
  // for each instruction format, look at the field mappings
  // if a field mapping is of a register type, but does not
  // have an associated register class associated with it,
  // flag it!
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    CoreGenInstFormat *IF = static_cast<CoreGenInstFormat *>(D1->FindNodeByIndex(i));
    if( IF->GetType() == CGInstF ){
      for( unsigned j=0; j<IF->GetNumFields(); j++ ){
        if( (IF->GetFieldType(IF->GetFieldName(j)) == CoreGenInstFormat::CGInstReg) &&
            (IF->GetFieldRegClass(IF->GetFieldName(j)) == nullptr) ){
          WriteMsg( "Identified a register field with no associated register class at InstFormat=" +
                    IF->GetName() + "; Field=" + IF->GetFieldName(j) );
          rtn = false;
        }// end inner if
      }// end for 'j'
    }// end if
  }// end node loop

  return rtn;
}
