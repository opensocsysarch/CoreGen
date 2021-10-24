//
// _SCNot_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCNot.h"

SCNot::SCNot() : SCIntrin(1,"NOT",true) {
  // no signals generated
}

SCNot::SCNot(unsigned NI, std::string K) : SCIntrin(NI,K,true) {
}

SCNot::~SCNot(){
}

Value *SCNot::codegen(){
  return nullptr;
}

bool SCNot::GetSigMap(CoreGenSigMap *Sigs,
                           Instruction &I,
                           std::string Inst){
  return Sigs->GetSignal(Sigs->GetNumSignals()-1)->SetFusedType(FOP_NOT);
}

// EOF
