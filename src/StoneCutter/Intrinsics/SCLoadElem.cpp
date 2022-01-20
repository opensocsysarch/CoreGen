//
// _SCLoadElem_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCLoadElem.h"

SCLoadElem::SCLoadElem() : SCIntrin(2,"LOADELEM",false) {
}

SCLoadElem::SCLoadElem(unsigned NI, std::string K) : SCIntrin(NI,K, false) {
}

SCLoadElem::~SCLoadElem(){
}

Value *SCLoadElem::codegen(){
  return nullptr;
}

bool SCLoadElem::GetSigMap(CoreGenSigMap *Sigs,
                           Instruction &I,
                           std::string Inst){
  unsigned width = DeriveWidth(I);
  if( !width ) {
    ErrMsg = "Invalid Width: Width cannot be 0";
  }

  std::string Str;
  if( MDNode *N = I.getMetadata("pipe.pipeName")) {
    Str = cast<MDString>(N->getOperand(0))->getString().str();
  }

  return Sigs->InsertSignal(new SCSig(MEM_READ,width,Inst,Str) );
}

// EOF
