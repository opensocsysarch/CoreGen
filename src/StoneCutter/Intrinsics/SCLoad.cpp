//
// _SCLoad_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCLoad.h"

SCLoad::SCLoad() : SCIntrin(1,"LOAD") {
}

SCLoad::SCLoad(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCLoad::~SCLoad(){
}

Value *SCLoad::codegen(){
  return nullptr;
}

// EOF
