//
// _SCCompress_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCCompress.h"

SCCompress::SCCompress() : SCIntrin(1,"COMPRESS",false) {
  ISignals.push_back(new SCSig(BR_NE));
  ISignals.push_back(new SCSig(ALU_ADD));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(ALU_AND));
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_OR));
}

SCCompress::SCCompress(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(BR_NE));
  ISignals.push_back(new SCSig(ALU_ADD));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(ALU_AND));
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_OR));
}

SCCompress::~SCCompress(){
}

Value *SCCompress::codegen(){
  return nullptr;
}

// EOF
