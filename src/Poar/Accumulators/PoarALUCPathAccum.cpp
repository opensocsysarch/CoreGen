//
// _PoarALUCPathAccum_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarALUCPathAccum.h"

uint64_t SigTypeToWidth( SigType T ){
  unsigned Type = (unsigned)(T);
  uint64_t Width = 0x00ull;

  if( Type == 0 ){
    return 1;   // we need a single bit
  }else{
    uint64_t TT = (uint64_t)(Type);
    while( TT > 0 ){
      Width++;
      TT = TT >> 1;
    }
  }

  return Width;
}

bool PoarALUCPathAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: pull the signal map
  CoreGenSigMap *SM = GetSignalMap();

  // Stage 2: Setup a vector of pipelines, walk each pipeline stage and determine
  //          the data path width of each incoming stage
  std::vector<std::string> Pipes = SM->GetPipelines();
  std::vector<SCSig *> SigVect;
  for(unsigned i=0; i<Pipes.size(); i++ ){
    for( unsigned j=0; j<SM->GetNumPipeStages(Pipes[i]); j++ ){
      std::string Stage = SM->GetPipelineStage(Pipes[i], j);
      SigVect = SM->GetSignalVectByPipeStage(Pipes[i],
                                             SM->GetPipelineStage(Pipes[i], j));
      for( unsigned k=0; k<SigVect.size(); k++ ){
        AddWidth( SigTypeToWidth( SigVect[k]->GetType() ) );
      }// for ( k.. )
    }// for ( j.. )
  }// for( i.. )

  return true;
}

// EOF
