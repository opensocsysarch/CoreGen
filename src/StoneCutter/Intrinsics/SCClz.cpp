//
// _SCClz_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCClz.h"

SCClz::SCClz() : SCIntrin(1,"CLZ") {
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_ADD));
}

SCClz::SCClz(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCClz::~SCClz(){
}

Value *SCClz::codegen(){
  return nullptr;
}

// EOF
