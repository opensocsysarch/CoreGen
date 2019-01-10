//
// _SCMerge_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCMerge.h"

SCMerge::SCMerge() : SCIntrin(2,"MERGE") {
}

SCMerge::SCMerge(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCMerge::~SCMerge(){
}

Value *SCMerge::codegen(){
  return nullptr;
}

// EOF
