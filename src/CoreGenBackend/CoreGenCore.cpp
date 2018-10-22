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
  : CoreGenNode(CGCore,N,E), Cache(NULL), ISA(I){
  InsertChild(static_cast<CoreGenNode *>(ISA));
}

CoreGenCore::~CoreGenCore(){
}

bool CoreGenCore::SetISA( CoreGenISA *I ){
  if( ISA ){
    DeleteChild(static_cast<CoreGenNode *>(ISA));
  }
  ISA = I;
  InsertChild(static_cast<CoreGenNode *>(ISA));
  return true;
}

bool CoreGenCore::InsertCache( CoreGenCache *C ){
  if( Cache ){
    DeleteChild(static_cast<CoreGenNode *>(Cache));
  }
  Cache = C;
  InsertChild(static_cast<CoreGenNode *>(Cache));
  return true;
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
  InsertChild(static_cast<CoreGenNode *>(RegClass));

  return true;
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
  InsertChild(static_cast<CoreGenNode *>(E));

  return true;
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

// EOF
