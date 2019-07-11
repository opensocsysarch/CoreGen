//
// _SCZext_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCZext.h"

SCZext::SCZext() : SCIntrin(2,"ZEXT",true) {
  // no signals
}

SCZext::SCZext(unsigned NI, std::string K) : SCIntrin(NI,K,true) {
}

SCZext::~SCZext(){
}

Value *SCZext::codegen(){
  return nullptr;
}

bool SCZext::GetSigMap(CoreGenSigMap *Sigs,
                           Instruction &I,
                           std::string Inst){
  return Sigs->GetSignal(Sigs->GetNumSignals()-1)->SetFusedType(FOP_ZEXT);
}

// EOF
