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

bool SCPipeBuilder::SplitRegisterIO(){

  // Determine whether we currently have any defined pipeline stages
  // If no, then define a minimum of five stages:
  // - N    = Fetch/PC_INCR
  // - N+1  = Register Read
  // - N+2  = Arith
  // - N+3  = Register Write
  // - N+4  = Memory
  if( PipeVect.size() == 0 ){
    PipeVect.push_back("FETCH");
    PipeVect.push_back("REG_READ");
    PipeVect.push_back("ARITH");
    PipeVect.push_back("REG_WRITE");
    PipeVect.push_back("MEMORY");

    // allocate the new matrix
    if( !AllocMat() ){
      this->PrintMsg( L_ERROR, "SplitRegisterIO: Failed to allocate matrix" );
      return false;
    }

    // lay out all the signals
    SCSig *Sig = nullptr;
    unsigned Idx = PipeVect.size();

    for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
      Sig = SigMap->GetSignal(i);
      if( Sig->GetType() == PC_INCR ){
        Idx = PipeToIdx("FETCH");
      }else if( Sig->isALUSig() ){
        Idx = PipeToIdx("ARITH");
      }else if( Sig->isMemSig() ){
        Idx = PipeToIdx("MEMORY");
      }else if( Sig->isRegSig() ){
        SigType Type = Sig->GetType();
        if( Type == REG_READ ){
          Idx = PipeToIdx("REG_READ");
        }else if( Type == REG_WRITE ){
          Idx = PipeToIdx("REG_WRITE");
        }else{
          Idx = PipeToIdx("ARITH");
        }
      }else if( Sig->isBranchSig() ){
        Idx = PipeToIdx("ARITH");
      }
      AdjMat[Idx][i] = 1;
    }
    return true;
  }

  // We have a mixture of currently defined pipeline states
  // We will need to derive which instructions have pipelines
  // containing register I/O operations and which do not.  For those
  // that do not, we need to "fit" them into the correct stages.

  // SplitPipes: instruction:pipeline:pipe_stage
  std::vector<std::tuple<std::string,std::string,std::string>> SplitPipes;
  std::vector<SCSig *> IVect;
  SCSig *Sig = nullptr;
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    Sig = SigMap->GetSignal(i);
    // if we find a register or memory signal, determine if it
    // shares a stage with an operation
    if( (Sig->isMemSig() || Sig->isRegSig()) && Sig->IsPipeDefined() ){
      // retrieve all the adjacent signals from the same instruction
      IVect = SigMap->GetSigVect(Sig->GetInst());
      for( unsigned j=0; j<IVect.size(); j++ ){
        if( (IVect[j] != Sig) &&
            (IVect[j]->IsPipeDefined()) &&
            (IVect[j]->isALUSig() || IVect[j]->isBranchSig()) &&
            (IVect[j]->GetPipeName() == Sig->GetPipeName()) ){
          // record the instruction : pipeline : pipe_stage
          SplitPipes.push_back( std::tuple<std::string,std::string,std::string>(
                                  IVect[j]->GetInst(),
                                  IVect[j]->GetPipeName(),
                                  Sig->GetPipeName() ) );
        } // end conditional
      } // for IVect loop
    } // end conditional
  } // end signal loop

  // determine if we have any candidate pipe stages to adjust
  if( SplitPipes.size() > 0 ){
  }

  return true;
}

unsigned SCPipeBuilder::PipeToIdx(std::string P){
  for( unsigned i=0; i<PipeVect.size(); i++ ){
    if( PipeVect[i] == P )
      return i;
  }

  return PipeVect.size();
}

bool SCPipeBuilder::Optimize(){
  bool (SCPipeBuilder::*Pass)() = nullptr;

  for( unsigned i=0; i<Enabled.size(); i++ ){
    Pass = Enabled[i].second;
    if( !(*this.*Pass)() ){
      this->PrintMsg( L_ERROR, "Subpass failed: " + Enabled[i].first );
      return false;
    }
  }

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

  // no reason to allocate a zero width matrix
  if( PipeVect.size() == 0 ){
    return true;
  }

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

bool SCPipeBuilder::EnableSubPasses(){
  // temporarily enable all the sub-passes
  Enabled.push_back(std::make_pair("SplitRegisterIO",
                                   &SCPipeBuilder::SplitRegisterIO) );
  return true;
}

bool SCPipeBuilder::IsSubPassEnabled(std::string Pass){
  for( unsigned i=0; i<Enabled.size(); i++ ){
    if( Enabled[i].first == Pass )
      return true;
  }
  return false;
}

bool SCPipeBuilder::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // retrieve the pipeline stage vector
  PipeVect = SigMap->GetPipeVect();
  InstVect = SigMap->GetInstVect();

  // Enable all the subpasses
  if( !EnableSubPasses() ){
    this->PrintMsg( L_ERROR, "Could not initialize sub-passes" );
    return false;
  }

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
