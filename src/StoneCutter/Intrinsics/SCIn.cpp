//
// _SCIn_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCIn.h"

SCIn::SCIn() : SCIntrin(2,"IN",false) {
}

SCIn::SCIn(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
}

SCIn::~SCIn(){

}

Value *SCIn::codegen(){
  return nullptr;
}

// EOF
