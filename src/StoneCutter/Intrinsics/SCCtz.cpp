//
// _SCCtz_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCCtz.h"

SCCtz::SCCtz() : SCIntrin(1,"CTZ") {
}

SCCtz::SCCtz(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCCtz::~SCCtz(){
}

Value *SCCtz::codegen(){
  return nullptr;
}

// EOF
