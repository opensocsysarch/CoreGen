//
// _SCStoreElem_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCStoreElem.h"

SCStoreElem::SCStoreElem() : SCIntrin(3,"STOREELEM") {
}

SCStoreElem::SCStoreElem(unsigned NI, std::string K) : SCIntrin(NI,K) {
}

SCStoreElem::~SCStoreElem(){
}

Value *SCStoreElem::codegen(){
  return nullptr;
}

bool SCStoreElem::GetSigMap(CoreGenSigMap *Sigs,
                            Instruction &I,
                            std::string Inst){
  unsigned width = 0;
  if( auto CInt = dyn_cast<ConstantInt>(I.getOperand(2)) ){
    width = (unsigned)(CInt->getSExtValue());
  }else{
    width = I.getOperand(2)->getType()->getIntegerBitWidth();
  }

  return Sigs->InsertSignal(new SCSig(MEM_WRITE,width,Inst) );
}

// EOF
