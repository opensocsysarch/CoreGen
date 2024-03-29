//
// _SCInsertS_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCInsertS.h"

SCInsertS::SCInsertS() : SCIntrin(3,"INSERTS",false) {
  ISignals.push_back(new SCSig(ALU_OR));
}

SCInsertS::SCInsertS(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(ALU_OR));
}

SCInsertS::~SCInsertS(){
}

Value *SCInsertS::codegen(){
  return nullptr;
}

// EOF
