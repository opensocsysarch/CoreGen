//
// _SCRotateL_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCRotateL.h"

SCRotateL::SCRotateL() : SCIntrin(2,"ROTL",false) {
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_SRL));
  ISignals.push_back(new SCSig(ALU_OR));
  ISignals.push_back(new SCSig(ALU_SUB));
}

SCRotateL::SCRotateL(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_SRL));
  ISignals.push_back(new SCSig(ALU_OR));
  ISignals.push_back(new SCSig(ALU_SUB));
}

SCRotateL::~SCRotateL(){
}

Value *SCRotateL::codegen(){
  return nullptr;
}

// EOF
