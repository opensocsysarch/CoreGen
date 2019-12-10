//
// _SCMax_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCMax.h"

SCMax::SCMax() : SCIntrin(2,"MAX",false) {
  ISignals.push_back(new SCSig(MUX_GE));
  ISignals.push_back(new SCSig(MUX));
}

SCMax::SCMax(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(MUX_GE));
  ISignals.push_back(new SCSig(MUX));
}

SCMax::~SCMax(){
}

Value *SCMax::codegen(){
  return nullptr;
}

// EOF
