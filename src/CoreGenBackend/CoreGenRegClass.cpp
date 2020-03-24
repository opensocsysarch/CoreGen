//
// _CoreGenRegClass_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"

CoreGenRegClass::CoreGenRegClass(CoreGenErrno *E)
  : CoreGenNode(CGRegC,E), ReadPorts(2), WritePorts(1) {
}

CoreGenRegClass::CoreGenRegClass(std::string N,CoreGenErrno *E)
  : CoreGenNode(CGRegC,N,E), ReadPorts(2), WritePorts(1) {
}

CoreGenRegClass::~CoreGenRegClass(){
}

CoreGenReg *CoreGenRegClass::GetReg( unsigned R ){
  if( R > (GetNumChild()-1) ){
    // out of bounds
    return nullptr;
  }
  return static_cast<CoreGenReg *>(GetChild(R));
}

CoreGenReg *CoreGenRegClass::GetRegByName(std::string Name){
  for(unsigned i = 0; i < this->GetNumReg(); i++){
    if(this->GetReg(i)->GetName() == Name){
      return this->GetReg(i);
    }
  }

  return nullptr;
}

CoreGenReg *CoreGenRegClass::GetRegByIndex(unsigned Idx){
  for( unsigned i=0; i<this->GetNumReg(); i++ ){
    if( this->GetReg(i)->GetIndex() == (int)(Idx) )
      return this->GetReg(i);
  }
  return nullptr;
}

unsigned CoreGenRegClass::GetMaxWidth(){
  unsigned max = 0;
  for( unsigned i=0; i<this->GetNumReg(); i++ ){
    CoreGenReg *R = static_cast<CoreGenReg *>(this->GetReg(i));
    if( (unsigned)(R->GetWidth()) > max )
      max = R->GetWidth();
  }
  return max;
}

bool CoreGenRegClass::InsertReg( CoreGenReg *R ){
  if( !R ){
    return false;
  }

  for( unsigned i=0; i<GetNumChild(); i++ ){
    if( GetChild(i)->GetName() == R->GetName() ){
      Errno->SetError( CGERR_ERROR,
       "Naming Collision : RegisterClass=" + this->GetName()
       + " : Existing Register Name=" + R->GetName() );
      return false;
    }
  }

  return InsertChild(static_cast<CoreGenNode *>(R));
}
// EOF
