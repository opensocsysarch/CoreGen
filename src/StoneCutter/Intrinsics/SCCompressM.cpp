//
// _SCCompressM_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCCompressM.h"

SCCompressM::SCCompressM() : SCIntrin(2,"COMPRESSM") {
}

SCCompressM::SCCompressM(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCCompressM::~SCCompressM(){
}

Value *SCCompressM::codegen(){
  return nullptr;
}

// EOF
