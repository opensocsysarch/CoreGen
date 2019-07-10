//
// _SCMaj_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCMaj.h"

SCMaj::SCMaj() : SCIntrin(3,"MAJ",false) {
  ISignals.push_back(new SCSig(MUX_GT));
  ISignals.push_back(new SCSig(MUX_LT));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(MUX));
}

SCMaj::SCMaj(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
  ISignals.push_back(new SCSig(MUX_GT));
  ISignals.push_back(new SCSig(MUX_LT));
  ISignals.push_back(new SCSig(ALU_SUB));
  ISignals.push_back(new SCSig(MUX));
}

SCMaj::~SCMaj(){
}

Value *SCMaj::codegen(){
  return nullptr;
}

// EOF
