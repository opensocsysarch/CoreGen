//
// _SCStore_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCStore.h"

SCStore::SCStore() : SCIntrin(2,"STORE") {
}

SCStore::SCStore(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCStore::~SCStore(){
}

Value *SCStore::codegen(){
  return nullptr;
}

// EOF
