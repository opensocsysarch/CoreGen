//
// _SCCompressM_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCCompressM.h"

SCCompressM::SCCompressM() : SCIntrin(2,"COMPRESSM") {
  ISignals.push_back(new SCSig(BR_NE));
  ISignals.push_back(new SCSig(ALU_ADD));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(ALU_AND));
  ISignals.push_back(new SCSig(BR_LT));
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_OR));
}

SCCompressM::SCCompressM(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCCompressM::~SCCompressM(){
}

Value *SCCompressM::codegen(){
  return nullptr;
}

// EOF
