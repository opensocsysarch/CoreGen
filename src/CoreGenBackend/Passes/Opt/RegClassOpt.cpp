//
// _RegClassOpt_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Opt/RegClassOpt.h"

RegClassOpt::RegClassOpt(std::ostream *O,
                         CoreGenDAG *D,
                         CoreGenErrno *E)
  : CoreGenPass(CGPassOpt,1,"RegClassOpt",
                "Optimizes the layout of register files",
                false,O,D,E) {
}

RegClassOpt::~RegClassOpt(){
}

void RegClassOpt::WalkRegClass(CoreGenRegClass *RC){
  // walk all the candidate registers
  for( unsigned i=0; i<RC->GetNumReg(); i++ ){
    if( RC->GetReg(i)->IsShared() ){
      RC->SetAttr(AttrSharedReg);
    }
    if( RC->GetReg(i)->IsTUSAttr() ){
      RC->SetAttr(AttrTUSReg);
    }
    if( (!RC->GetReg(i)->IsShared()) &&
        (!RC->GetReg(i)->IsTUSAttr()) ){
      RC->SetAttr(AttrPrivReg);
    }
  }
}


bool RegClassOpt::Execute(){
  // Get the correct DAG level: 1
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Lvel " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  // Walk all the nodes and find the register classes; examine the registers
  // within each class and mark the appropriate attributes
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    if( D1->FindNodeByIndex(i)->GetType() == CGRegC ){
      WalkRegClass(static_cast<CoreGenRegClass *>(D1->FindNodeByIndex(i)));
    }
  }

  return true;
}

// EOF
