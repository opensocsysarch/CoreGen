//
// _SCNop_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCNop.h"

SCNop::SCNop() : SCIntrin(0,"NOP",false) {
}

SCNop::SCNop(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(BR_N));
}

SCNop::~SCNop(){
}

Value *SCNop::codegen(){
  return nullptr;
}

// EOF
