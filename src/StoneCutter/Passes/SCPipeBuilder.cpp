//
// _SCPipeBuilder_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCPipeBuilder.h"

SCPipeBuilder::SCPipeBuilder(Module *TM,
                             SCOpts *O,
                             SCMsg *M)
  : SCPass("PipeBuilder","",TM,O,M) {
  AdjMat = nullptr;
}

SCPipeBuilder::~SCPipeBuilder(){
}

unsigned SCPipeBuilder::PipeToIdx(std::string P){
  for( unsigned i=0; i<PipeVect.size(); i++ ){
    if( PipeVect[i] == P )
      return i;
  }

  return PipeVect.size();
}

bool SCPipeBuilder::Optimize(){
  return true;
}

bool SCPipeBuilder::BuildMat(){

  SCSig *Sig = nullptr;
  unsigned Idx = PipeVect.size();

  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    Sig = SigMap->GetSignal(i);
    if( Sig->IsPipeDefined() ){
      Idx = PipeToIdx(Sig->GetPipeName());
      if( Idx == PipeVect.size() ){
        this->PrintMsg( L_ERROR, "Pipe stage unknown" );
        return false;
      }
      AdjMat[Idx][i] = 1;
    }
  }

  return true;
}

bool SCPipeBuilder::WriteSigMap(){
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    for( unsigned j=0; j<PipeVect.size(); j++ ){
      if( AdjMat[j][i] == 1 ){
        SigMap->GetSignal(i)->SetPipeName(PipeVect[j]);
      }
    }
  }
  return SigMap->WriteSigMap();
}

bool SCPipeBuilder::AllocMat(){
  if( AdjMat != nullptr )
    return false;

  AdjMat = new unsigned*[PipeVect.size()];
  if( AdjMat == nullptr )
    return false;

  for( unsigned i=0; i<PipeVect.size(); i++ ){
    AdjMat[i] = new unsigned[SigMap->GetNumSignals()];
    if( AdjMat[i] == nullptr )
      return false;
  }

  for( unsigned i=0; i<PipeVect.size(); i++ ){
    for( unsigned j=0; j<SigMap->GetNumSignals(); j++ ){
      AdjMat[i][j] = 0;
    }
  }
  return true;
}

bool SCPipeBuilder::FreeMat(){
  if( AdjMat == nullptr )
    return true;

  if( AdjMat != nullptr ){
    for( unsigned i=0; i<PipeVect.size(); i++ ){
      delete [] AdjMat[i];
    }
    delete [] AdjMat;
  }

  return true;
}

bool SCPipeBuilder::InitAttrs(){
  for( unsigned i=0; i<PipeVect.size(); i++ ){
    std::vector<std::string> PV = GetPipelineAttrs(PipeVect[i]);
    for( unsigned j=0; j<PV.size(); j++ ){
      AttrMap.push_back( std::make_pair(PipeVect[i],PV[j]) );
    }
  }

  return true;
}

bool SCPipeBuilder::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // retrieve the pipeline stage vector
  PipeVect = SigMap->GetPipeVect();
  InstVect = SigMap->GetInstVect();

  // Initalize the attribute map
  if( !InitAttrs() ){
    this->PrintMsg( L_ERROR, "Could not initialize attributes" );
    return false;
  }

  // TODO: what if no pipeline stages are defined?
  //       different optimization path?

  // allocate the matrix
  if( !AllocMat() ){
    this->PrintMsg( L_ERROR, "Could not allocate matrix" );
    return false;
  }

  // build the pipeline matrix
  if( !BuildMat() ){
    this->PrintMsg( L_ERROR, "Could not build matrix representation of pipeline" );
    FreeMat();
    return false;
  }

  // execute optimizations
  if( !Optimize() ){
    this->PrintMsg( L_ERROR, "Encountered errors executing the the optimization phases" );
    FreeMat();
    return false;
  }

  // write everything back to the sigmapA
  if( !WriteSigMap() ){
    this->PrintMsg( L_ERROR, "Could not write signal map for the prescribed pipeline" );
    FreeMat();
    return false;
  }

  // free the matrix
  if( !FreeMat() ){
    this->PrintMsg( L_ERROR, "Could not free adjacency matrix" );
    return false;
  }

  return true;
}

// EOF
