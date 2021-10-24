//
// _PoarALURegAccum_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarALURegAccum.h"

bool PoarALURegAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: pull the signal map
  CoreGenSigMap *SM = GetSignalMap();

  // Stage 2: walk all the registers and determine their width in bits
  for( unsigned i=0; i<SM->GetNumTemps(); i++ ){
    AddWidth(SM->GetTemp(i)->GetWidth());
  }

  return true;
}

// EOF
