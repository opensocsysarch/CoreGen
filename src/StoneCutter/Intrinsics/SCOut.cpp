//
// _SCOut_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCOut.h"

SCOut::SCOut() : SCIntrin(1,"OUT",false) {
}

SCOut::SCOut(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
}

SCOut::~SCOut(){
}

Value *SCOut::codegen(){
  return nullptr;
}

// EOF
