//
// _SCPopcount_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCPopcount.h"

SCPopcount::SCPopcount() : SCIntrin(1,"POPCOUNT",false) {
  ISignals.push_back(new SCSig(BR_NE));
  ISignals.push_back(new SCSig(ALU_ADD));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(ALU_AND));
}

SCPopcount::SCPopcount(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(BR_NE));
  ISignals.push_back(new SCSig(ALU_ADD));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(ALU_AND));
}

SCPopcount::~SCPopcount(){
}

Value *SCPopcount::codegen(){
  return nullptr;
}

// EOF
