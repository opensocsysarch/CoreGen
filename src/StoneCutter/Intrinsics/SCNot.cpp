//
// _SCNot_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCNot.h"

SCNot::SCNot() : SCIntrin(1,"NOT") {
}

SCNot::SCNot(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCNot::~SCNot(){
}

Value *SCNot::codegen(){
  return nullptr;
}

// EOF
