//
// _CoreGenEncoding_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"

CoreGenEncoding::CoreGenEncoding( CoreGenErrno *E)
  : CoreGenNode(CGEnc,E), Encoding(0x00ull), Len(0) {
}

CoreGenEncoding::CoreGenEncoding( uint64_t Value, unsigned L,
                                  std::string F, std::string I,
                                  CoreGenErrno *E )
  : CoreGenNode(CGEnc,E), Len(L), Field(F){
  uint64_t Mask = 0x00ull;
  for( unsigned i=0; i<(uint64_t)(L); i++ ){
    Mask |= (uint64_t)(1<<i);
  }
  Encoding = (Value & Mask);
  this->SetName(I+":"+F);
}

CoreGenEncoding::~CoreGenEncoding(){
}

bool CoreGenEncoding::SetEncoding( uint64_t Value, unsigned L,
                                   std::string F, std::string I ){
  uint64_t Mask = 0x00ull;
  for( unsigned i=0; i<(uint64_t)(L); i++ ){
    Mask |= (uint64_t)(1<<i);
  }
  Encoding = (Value & Mask);
  Len = L;
  Field = F;
  this->SetName(I+":"+F);
  return true;
}

// EOF
