//
// _SCSext_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Intrinsics/SCSext.h"

SCSext::SCSext() : SCIntrin(2,"SEXT",true) {
  // no signals
}

SCSext::SCSext(unsigned NI, std::string K) : SCIntrin(NI,K,true) {
}

SCSext::~SCSext(){
}

Value *SCSext::codegen(){
  return nullptr;
}

bool SCSext::GetSigMap(CoreGenSigMap *Sigs,
                           Instruction &I,
                           std::string Inst){
  return Sigs->GetSignal(Sigs->GetNumSignals()-1)->SetFusedType(FOP_SEXT);
}

// EOF
