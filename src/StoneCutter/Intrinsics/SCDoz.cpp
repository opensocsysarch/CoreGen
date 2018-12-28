//
// _SCDoz_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCDoz.h"

SCDoz::SCDoz() : SCIntrin(2,"DOZ") {
}

SCDoz::SCDoz(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCDoz::~SCDoz(){
}

Value *SCDoz::codegen(){
  return nullptr;
}

// EOF
