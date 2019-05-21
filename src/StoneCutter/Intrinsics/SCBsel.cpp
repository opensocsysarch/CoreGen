//
// _SCBsel_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCBsel.h"

SCBsel::SCBsel() : SCIntrin(3,"BSEL") {
}

SCBsel::SCBsel(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCBsel::~SCBsel(){
}

Value *SCBsel::codegen(){
  return nullptr;
}

// EOF
