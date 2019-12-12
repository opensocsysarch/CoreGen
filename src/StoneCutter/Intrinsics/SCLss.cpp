//
// _SCLss_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCLss.h"

SCLss::SCLss() : SCIntrin(1,"LSS",false) {
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_ADD));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(ALU_OR));
}

SCLss::SCLss(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_ADD));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(ALU_OR));
}

SCLss::~SCLss(){
}

Value *SCLss::codegen(){
  return nullptr;
}

// EOF
