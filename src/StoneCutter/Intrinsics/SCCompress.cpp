//
// _SCCompress_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCCompress.h"

SCCompress::SCCompress() : SCIntrin(1,"COMPRESS") {
}

SCCompress::SCCompress(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCCompress::~SCCompress(){
}

Value *SCCompress::codegen(){
  return nullptr;
}

// EOF
