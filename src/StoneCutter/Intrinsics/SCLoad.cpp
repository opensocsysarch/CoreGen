//
// _SCLoad_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCLoad.h"

SCLoad::SCLoad() : SCIntrin(1,"LOAD",false) {
}

SCLoad::SCLoad(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
}

SCLoad::~SCLoad(){
}

Value *SCLoad::codegen(){
  return nullptr;
}

bool SCLoad::GetSigMap(CoreGenSigMap *Sigs,
                       Instruction &I,
                       std::string Inst){
  unsigned width = I.getOperand(0)->getType()->getIntegerBitWidth();
  std::string Str;
  if( MDNode *N = I.getMetadata("pipe.pipeName")) {
    Str = cast<MDString>(N->getOperand(0))->getString().str();
  }
  return Sigs->InsertSignal(new SCSig(MEM_READ,width,Inst,Str) );
}

// EOF
