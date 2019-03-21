//
// _SCLss_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCLss.h"

SCLss::SCLss() : SCIntrin(1,"LSS") {
}

SCLss::SCLss(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCLss::~SCLss(){
}

Value *SCLss::codegen(){
  return nullptr;
}

// EOF
