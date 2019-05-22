//
// _SCReverse_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCReverse.h"

SCReverse::SCReverse() : SCIntrin(1,"REVERSE") {
  ISignals.push_back(new SCSig(ALU_SLTU));
  ISignals.push_back(new SCSig(ALU_OR));
  ISignals.push_back(new SCSig(ALU_AND));
  ISignals.push_back(new SCSig(BR_GT));
}

SCReverse::SCReverse(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCReverse::~SCReverse(){
}

Value *SCReverse::codegen(){
  return nullptr;
}

// EOF
