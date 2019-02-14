//
// _SCExtractS_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCExtractS.h"

SCExtractS::SCExtractS() : SCIntrin(2,"EXTRACTS") {
}

SCExtractS::SCExtractS(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCExtractS::~SCExtractS(){
}

Value *SCExtractS::codegen(){
  return nullptr;
}

// EOF
