//
// _SCStore_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

bool SCStore::GetSigMap(CoreGenSigMap *Sigs, Instruction &I){
  unsigned width = I.getOperand(0)->getType()->getIntegerBitWidth();
  ISignals.push_back(new SCSig(MEM_WRITE,width) );
  return true;
}

// EOF
