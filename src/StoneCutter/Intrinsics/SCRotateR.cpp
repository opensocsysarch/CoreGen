//
// _SCRotateR_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCRotateR.h"

SCRotateR::SCRotateR() : SCIntrin(2,"ROTR",false) {
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_SRL));
  ISignals.push_back(new SCSig(ALU_OR));
  ISignals.push_back(new SCSig(ALU_SUB));
}

SCRotateR::SCRotateR(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_SRL));
  ISignals.push_back(new SCSig(ALU_OR));
  ISignals.push_back(new SCSig(ALU_SUB));
}

SCRotateR::~SCRotateR(){
}

Value *SCRotateR::codegen(){
  return nullptr;
}

// EOF
