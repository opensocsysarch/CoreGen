//
// _SCZext_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCZext.h"

SCZext::SCZext() : SCIntrin(2,"ZEXT") {
}

SCZext::SCZext(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCZext::~SCZext(){
}

Value *SCZext::codegen(){
  return nullptr;
}

// EOF
