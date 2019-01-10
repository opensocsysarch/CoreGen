//
// _SCInsertS_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCInsertS.h"

SCInsertS::SCInsertS() : SCIntrin(3,"INSERTS") {
}

SCInsertS::SCInsertS(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCInsertS::~SCInsertS(){
}

Value *SCInsertS::codegen(){
  return nullptr;
}

// EOF
