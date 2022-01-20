//
// _SCNeg_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCNeg.h"

SCNeg::SCNeg() : SCIntrin(1,"NEG",true) {
  // no signals generated
}

SCNeg::SCNeg(unsigned NI, std::string K) : SCIntrin(NI,K,true) {
}

SCNeg::~SCNeg(){
}

Value *SCNeg::codegen(){
  return nullptr;
}

bool SCNeg::GetSigMap(CoreGenSigMap *Sigs,
                           Instruction &I,
                           std::string Inst){
  return Sigs->GetSignal(Sigs->GetNumSignals()-1)->SetFusedType(FOP_NEG);
}

// EOF
