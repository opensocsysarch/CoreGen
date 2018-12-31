//
// _SCMax_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCMax.h"

SCMax::SCMax() : SCIntrin(2,"MAX") {
}

SCMax::SCMax(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCMax::~SCMax(){
}

Value *SCMax::codegen(){
  return nullptr;
}

// EOF
