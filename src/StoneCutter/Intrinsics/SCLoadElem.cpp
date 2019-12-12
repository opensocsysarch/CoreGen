//
// _SCLoadElem_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCLoadElem.h"

SCLoadElem::SCLoadElem() : SCIntrin(2,"LOADELEM",false) {
}

SCLoadElem::SCLoadElem(unsigned NI, std::string K) : SCIntrin(NI,K,false) {
}

SCLoadElem::~SCLoadElem(){
}

Value *SCLoadElem::codegen(){
  return nullptr;
}

bool SCLoadElem::GetSigMap(CoreGenSigMap *Sigs,
                           Instruction &I,
                           std::string Inst){
  unsigned width = 0;
  if( auto CInt = dyn_cast<ConstantInt>(I.getOperand(1)) ){
    width = (unsigned)(CInt->getSExtValue());
  }else{
    width = I.getOperand(1)->getType()->getIntegerBitWidth();
  }

  return Sigs->InsertSignal(new SCSig(MEM_READ,width,Inst) );
}

// EOF
