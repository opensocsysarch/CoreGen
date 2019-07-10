//
// _SCInsertZ_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCInsertZ.h"

SCInsertZ::SCInsertZ() : SCIntrin(3,"INSERTZ",false) {
  ISignals.push_back(new SCSig(ALU_OR));
}

SCInsertZ::SCInsertZ(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(ALU_OR));
}

SCInsertZ::~SCInsertZ(){
}

Value *SCInsertZ::codegen(){
  return nullptr;
}

// EOF
