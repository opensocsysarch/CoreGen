//
// _SCConcat_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCConcat.h"

SCConcat::SCConcat() : SCIntrin(2,"CONCAT",false) {
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_OR));
}

SCConcat::SCConcat(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(ALU_SLL));
  ISignals.push_back(new SCSig(ALU_OR));
}

SCConcat::~SCConcat(){
}

Value *SCConcat::codegen(){
  return nullptr;
}

// EOF
