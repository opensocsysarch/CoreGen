//
// _PoarALUAccum_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarALUAccum.h"

bool PoarALUAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: pull the signal map
  CoreGenSigMap *SM = GetSignalMap();

  // Stage 2: Setup a vector of pipelines with their respective values
  //          This will allow us to multiply the ALU cost for superscalar designs
  std::vector<std::pair<std::string,uint64_t>> PipeToCost;
  std::vector<std::string> Pipes = SM->GetPipelines();
  for( unsigned i=0; i<Pipes.size(); i++ ){
    PipeToCost.push_back(std::make_pair(Pipes[i],0x00ull));
  }

  // Stage 3: Walk each pipeline and accumulate the signals in each stage
  std::vector<SCSig *> SigVect;
  for( unsigned i=0; i<Pipes.size(); i++ ){
    for( unsigned j=0; j<SM->GetNumPipeStages(Pipes[i]); j++ ){
      std::string Stage = SM->GetPipelineStage(Pipes[i], j);
      SigVect = SM->GetSignalVectByPipeStage(Pipes[i],
                                             SM->GetPipelineStage(Pipes[i], j));
      for( unsigned k=0; k<SigVect.size(); k++ ){
        PipeToCost[i].second += (uint64_t)(SigVect[k]->GetWidth());
      }// for( k.. )
    }// for( j.. )
  }// for( i.. )

  // Stage 4: Accumulate into the final value
  for( unsigned i=0; i<PipeToCost.size(); i++ ){
    AddWidth(PipeToCost[i].second);
  }

  return true;
}

// EOF
