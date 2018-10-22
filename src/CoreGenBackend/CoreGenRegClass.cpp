//
// _CoreGenRegClass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"

CoreGenRegClass::CoreGenRegClass(CoreGenErrno *E)
  : CoreGenNode(CGRegC,E){
}

CoreGenRegClass::CoreGenRegClass(std::string N,CoreGenErrno *E)
  : CoreGenNode(CGRegC,N,E){
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
