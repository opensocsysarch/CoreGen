//
// _InstTable_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Data/InstTable.h"

InstTable::InstTable(std::ostream *O,
                     CoreGenDAG *D,
                     CoreGenErrno *E)
  : CoreGenPass(CGPassData,1,"InstTable",
                "Prints instruction and register tables",false,O,D,E){
}

InstTable::~InstTable(){
}

bool InstTable::Execute(){
  // Get the correct DAG Level 3
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  // TODO: print the register classes; registers and instruction tables

  return true;
}

// EOF
