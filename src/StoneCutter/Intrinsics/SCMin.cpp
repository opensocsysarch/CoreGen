//
// _SCMin_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCMin.h"

SCMin::SCMin() : SCIntrin(2,"MIN",false) {
  ISignals.push_back(new SCSig(MUX_LE));
  ISignals.push_back(new SCSig(MUX));
}

SCMin::SCMin(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(MUX_LE));
  ISignals.push_back(new SCSig(MUX));
}

SCMin::~SCMin(){
}

Value *SCMin::codegen(){
  return nullptr;
}

// EOF
