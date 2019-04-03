//
// _SCConcat_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCConcat.h"

SCConcat::SCConcat() : SCIntrin(2,"CONCAT") {
}

SCConcat::SCConcat(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCConcat::~SCConcat(){
}

Value *SCConcat::codegen(){
  return nullptr;
}

// EOF
