//
// _SCPopcount_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCPopcount.h"

SCPopcount::SCPopcount() : SCIntrin(1,"POPCOUNT") {
}

SCPopcount::SCPopcount(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCPopcount::~SCPopcount(){
}

Value *SCPopcount::codegen(){
  return nullptr;
}

// EOF
