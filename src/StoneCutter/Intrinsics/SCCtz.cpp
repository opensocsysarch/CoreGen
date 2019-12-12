//
// _SCCtz_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCCtz.h"

SCCtz::SCCtz() : SCIntrin(1,"CTZ",false) {
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SRL));
  ISignals.push_back(new SCSig(ALU_ADD));
}

SCCtz::SCCtz(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SRL));
  ISignals.push_back(new SCSig(ALU_ADD));
}

SCCtz::~SCCtz(){
}

Value *SCCtz::codegen(){
  return nullptr;
}

// EOF
