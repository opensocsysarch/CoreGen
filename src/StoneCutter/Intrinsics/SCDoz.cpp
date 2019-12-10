//
// _SCDoz_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCDoz.h"

SCDoz::SCDoz() : SCIntrin(2,"DOZ",false) {
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(MUX_LT));
}

SCDoz::SCDoz(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(MUX_LT));
}

SCDoz::~SCDoz(){
}

Value *SCDoz::codegen(){
  return nullptr;
}

// EOF
