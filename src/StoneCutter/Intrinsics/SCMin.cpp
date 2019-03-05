//
// _SCMin_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCMin.h"

SCMin::SCMin() : SCIntrin(2,"MIN") {
}

SCMin::SCMin(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCMin::~SCMin(){
}

Value *SCMin::codegen(){
  return nullptr;
}

// EOF
