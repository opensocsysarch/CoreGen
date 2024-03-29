//
// _SCStore_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCStore.h"

SCStore::SCStore() : SCIntrin(2,"STORE",false) {
}

SCStore::SCStore(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
}

SCStore::~SCStore(){
}

Value *SCStore::codegen(){
  return nullptr;
}

bool SCStore::GetSigMap(CoreGenSigMap *Sigs,
                        Instruction &I,
                        std::string Inst){
  unsigned width = I.getOperand(0)->getType()->getIntegerBitWidth();
  std::string Str;
  if( MDNode *N = I.getMetadata("pipe.pipeName")) {
    Str = cast<MDString>(N->getOperand(0))->getString().str();
  }
  return Sigs->InsertSignal(new SCSig(MEM_WRITE,width,Inst,Str) );
}

// EOF
