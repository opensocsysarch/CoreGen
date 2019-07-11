//
// _CoreGenSoC_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenSoC.h"


CoreGenSoC::CoreGenSoC(std::string N, CoreGenErrno *E)
  : CoreGenNode(CGSoc,N,E){
}

CoreGenSoC::~CoreGenSoC(){
}

bool CoreGenSoC::InsertCore( CoreGenCore *C ){
  if( C == nullptr ){
    return false;
  }
  // check for collisions
  for( unsigned i=0; i<Cores.size(); i++ ){
    if( Cores[i]->GetName() == C->GetName() ){
      // don't insert a collision
      return true;
    }
  }

  // insert it
  Cores.push_back( C );
  return InsertChild(static_cast<CoreGenNode *>(C));
}

bool CoreGenSoC::DeleteCore( CoreGenCore *C ){
  if( !C )
    return false;

  for( unsigned i=0; i<Cores.size(); i++ ){
    if( Cores[i] == C ){
      Cores.erase(Cores.begin()+i);
      return true;
    }
  }
  return false;
}

CoreGenCore *CoreGenSoC::GetCore( unsigned C ){
  if( C > (Cores.size()-1)){
    Errno->SetError( CGERR_ERROR, "Core index "
                     + std::to_string(C) + " out of bounds" );
    return NULL;
  }
  return Cores[C];
}

// EOF
