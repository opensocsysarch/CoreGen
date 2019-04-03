//
// _SCLoadElem_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCLoadElem.h"

SCLoadElem::SCLoadElem() : SCIntrin(2,"LOADELEM") {
}

SCLoadElem::SCLoadElem(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCLoadElem::~SCLoadElem(){
}

Value *SCLoadElem::codegen(){
  return nullptr;
}

// EOF
