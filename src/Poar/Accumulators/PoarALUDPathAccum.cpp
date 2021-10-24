//
// _PoarALUDPathAccum_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarALUDPathAccum.h"

bool PoarALUDPathAccum::Accum(){
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
        AddWidth( (uint64_t)(SigVect[k]->GetNumInputs()) *
                  (uint64_t)(SigVect[k]->GetWidth()) );
      }// for ( k.. )
    }// for ( j.. )
  }// for( i.. )

  return true;
}

// EOF
