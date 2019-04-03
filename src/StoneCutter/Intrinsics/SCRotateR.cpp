//
// _SCRotateR_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCRotateR.h"

SCRotateR::SCRotateR() : SCIntrin(2,"ROTR") {
}

SCRotateR::SCRotateR(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCRotateR::~SCRotateR(){
}

Value *SCRotateR::codegen(){
  return nullptr;
}

// EOF
