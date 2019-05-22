//
// _SCSext_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCSext.h"

SCSext::SCSext() : SCIntrin(2,"SEXT") {
  // no signals
}

SCSext::SCSext(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCSext::~SCSext(){
}

Value *SCSext::codegen(){
  return nullptr;
}

// EOF
