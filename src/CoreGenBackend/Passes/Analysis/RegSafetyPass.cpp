//
// _RegSafetyPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/RegSafetyPass.h"

RegSafetyPass::RegSafetyPass(std::ostream *O,
                             CoreGenDAG *D,
                             CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,3,"RegSafetyPass",
                "Checks various safety parameters of the register files",
                false,O,D,E ){
}

RegSafetyPass::~RegSafetyPass(){
}

// Look for registers with unitialized indices
bool RegSafetyPass::FindMissingRegIndices(CoreGenReg *R){
  if( !R->IsIndexSet() ){
    WriteMsg( "Identified a register with no initialized index; Register=" +
              R->GetName() );
    return false;
  }
  return true;
}

// Look for registers whose initialized values are beyond the width
bool RegSafetyPass::FindRegWidthRestrictions(CoreGenReg *R){
  if( R->IsFixedValue() ){
    int Width = R->GetWidth();
    if( Width <= 64 ){
      // sub-64 bit
      uint64_t Val = 0x00ull;
      for( int i=0; i<Width; i++ ){
        Val |= (1<<i);
      }

      std::vector<uint64_t> FixedVals;
      if( !R->GetFixedVals(FixedVals) ){
        WriteMsg( "Error retrieving fixed values for Register=" + R->GetName());
        return false;
      }

      if( FixedVals.size() == 0 ){
        WriteMsg( "No fixed values found for Register=" + R->GetName());
        return false;
      }

      if( FixedVals[0] > Val ){
        WriteMsg( "Fixed value for Register=" + R->GetName() +
                  " exceeds maximum register width" );
        return false;
      }
    }else{
      // multi-precision
    }
  }
  return true;
}

bool RegSafetyPass::Execute(){
  // Get the correct DAG level
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D3 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;


  for( unsigned i=0; i<D3->GetDimSize(); i++ ){
    CoreGenReg *REG = static_cast<CoreGenReg *>(D3->FindNodeByIndex(i));
    if( REG->GetType() == CGReg ){
      if( !FindMissingRegIndices(REG) ){
        rtn = false;
      }
      if( !FindRegWidthRestrictions(REG) ){
        rtn = false;
      }
    }
  }

  return rtn;
}

// EOF
