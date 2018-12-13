//
// _CoreGenCore_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenCore.h"

CoreGenCore::CoreGenCore( std::string N, CoreGenISA *I, CoreGenErrno *E)
  : CoreGenNode(CGCore,N,E), Cache(NULL), ISA(I), ThreadUnits(1){
  InsertChild(static_cast<CoreGenNode *>(ISA));
}

CoreGenCore::~CoreGenCore(){
}

bool CoreGenCore::SetISA( CoreGenISA *I ){
  if( ISA ){
    DeleteChild(static_cast<CoreGenNode *>(ISA));
  }
  ISA = I;
  return InsertChild(static_cast<CoreGenNode *>(ISA));
}

bool CoreGenCore::InsertCache( CoreGenCache *C ){
  if( Cache ){
    DeleteChild(static_cast<CoreGenNode *>(Cache));
  }
  Cache = C;
  return InsertChild(static_cast<CoreGenNode *>(Cache));
}

bool CoreGenCore::InsertRegClass( CoreGenRegClass *RegClass ){
  // check for a collision
  for( unsigned i=0; i<Regs.size(); i++ ){
    if( Regs[i] == RegClass ){
      // already present
      return true;
    }
  }

  // insert it
  Regs.push_back(RegClass);
  return InsertChild(static_cast<CoreGenNode *>(RegClass));
}

bool CoreGenCore::InsertExt( CoreGenNode *E ){
  // check for a collision
  for( unsigned i=0; i<Exts.size(); i++ ){
    if( Exts[i] == E ){
      // already present
      return true;
    }
  }

  // insert it
  Exts.push_back(E);
  return InsertChild(static_cast<CoreGenNode *>(E));
}

CoreGenRegClass *CoreGenCore::GetRegClass( unsigned C ){
  if( C > (Regs.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Register class " + std::to_string(C)
              + " out of bounds");
    return NULL;
  }

  return Regs[C];
}

CoreGenNode *CoreGenCore::GetExt( unsigned E ){
  if( E > (Exts.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Extension" + std::to_string(E)
              + " out of bounds");
    return NULL;
  }

  return Exts[E];
}

bool CoreGenCore::SetNumThreadUnits( unsigned TU ){
  if( TU == 0 ){
    Errno->SetError( CGERR_ERROR, "Cannot set thread units to zero" );
    return false;
  }
  ThreadUnits = TU;
  return true;
}

// EOF
