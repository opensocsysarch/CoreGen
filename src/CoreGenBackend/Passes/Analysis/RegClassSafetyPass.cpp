//
// _RegClassSafetyPass_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/RegClassSafetyPass.h"

RegClassSafetyPass::RegClassSafetyPass(std::ostream *O,
                                     CoreGenDAG *D,
                                     CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"RegClassSafetyPass",
                "Examines IR and warns on issues with RegClasses",
                false,O,D,E) {
}

RegClassSafetyPass::~RegClassSafetyPass(){
}

bool RegClassSafetyPass::EmptyRegClass(CoreGenNode *N){
  CoreGenRegClass *R = static_cast<CoreGenRegClass *>(N);
  if( R->GetNumReg() == 0 ){
    WriteMsg("Encountered register class with no registers : " +
             R->GetName() );
    return false;
  }
  return true;
}

bool RegClassSafetyPass::RegNameCollision(CoreGenNode *N){
  CoreGenRegClass *R = static_cast<CoreGenRegClass *>(N);
  std::vector<std::string> Names;
  for( unsigned i=0; i<R->GetNumReg(); i++ ){
    Names.push_back(R->GetReg(i)->GetName());
  }
  std::sort(Names.begin(), Names.end());
  std::vector<std::string>::iterator it;
  it = std::adjacent_find(Names.begin(),Names.end());
  if( it != Names.end() ){
    WriteMsg("Register Name collision in RegisterClass=" +
             R->GetName() + "; RegisterName=" +
             *it );
    return false;
  }
  return true;
}

bool RegClassSafetyPass::RegSizeTest(CoreGenNode *N){
  CoreGenRegClass *R = static_cast<CoreGenRegClass *>(N);
  std::vector<int> Widths;
  for( unsigned i=0; i<R->GetNumReg(); i++ ){
    Widths.push_back(R->GetReg(i)->GetWidth());
  }

  std::sort( Widths.begin(), Widths.end() );
  int unique = std::unique(Widths.begin(),Widths.end()) - Widths.begin();
  if( unique > 1 ){
    WriteMsg("RegisterClass=" + R->GetName() + " contains multiple sized registers which may induce inefficient Chisel output");
    return false;
  }

  return true;
}

bool RegClassSafetyPass::PseudoNameCollision(CoreGenNode *N){
  CoreGenRegClass *R = static_cast<CoreGenRegClass *>(N);
  std::vector<std::string> PNames;
  for( unsigned i=0; i<R->GetNumReg(); i++ ){
    std::string PName = R->GetReg(i)->GetPseudoName();
    if( PName.length() > 0 ){
      PNames.push_back(PName);
    }
  }
  std::sort(PNames.begin(), PNames.end());
  std::vector<std::string>::iterator it;
  it = std::adjacent_find(PNames.begin(),PNames.end());
  if( it != PNames.end() ){
    WriteMsg("PseudoName collision in RegisterClass=" +
             R->GetName() + "; PseudoName=" +
             *it );
    return false;
  }
  return true;
}

bool RegClassSafetyPass::ReadWritePortTest( CoreGenNode *N ){
  CoreGenRegClass *R = static_cast<CoreGenRegClass *>(N);
  bool RW = false;
  bool rtn = true;
  for( unsigned i=0; i<R->GetNumReg(); i++ ){
    if( R->GetReg(i)->IsRWAttr() )
      RW = true;
  }

  // Read ports must be > 0
  if( R->GetReadPorts() == 0 ){
    WriteMsg("Register class must have at least one read port; RegisterClass=" +
             R->GetName() );
    rtn = false;
  }

  // if there are any RW registers, then we must have a write port
  if( RW ){
    if( R->GetWritePorts() == 0 ){
      WriteMsg("Register classes with RW I/O must have at least write port; RegisterClass=" +
             R->GetName() );
      rtn = false;
    }
  }

  return rtn;
}

bool RegClassSafetyPass::Execute(){
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


  // walk all the nodes and test the register classes
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    if( D1->FindNodeByIndex(i)->GetType() == CGRegC ){
      // empty register classes
      if( !EmptyRegClass( D1->FindNodeByIndex(i) ) ){
        rtn = false;
      }
      // register name collisions
      if( !RegNameCollision( D1->FindNodeByIndex(i) ) ){
        rtn = false;
      }
      // register pseudo name collisions
      if( !PseudoNameCollision( D1->FindNodeByIndex(i) ) ){
        rtn = false;
      }
      // register size inefficiencies
      if( !RegSizeTest( D1->FindNodeByIndex(i) ) ){
        rtn = false;
      }
      // register class read/write port tests
      if( !ReadWritePortTest( D1->FindNodeByIndex(i) ) ){
        rtn = false;
      }
    }
  }

  return rtn;
}

// EOF
