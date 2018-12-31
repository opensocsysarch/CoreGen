//
// _SCRotateL_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCRotateL.h"

SCRotateL::SCRotateL() : SCIntrin(2,"ROTL") {
}

SCRotateL::SCRotateL(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCRotateL::~SCRotateL(){
}

Value *SCRotateL::codegen(){
  return nullptr;
}

// EOF
