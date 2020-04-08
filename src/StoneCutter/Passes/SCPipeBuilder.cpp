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

bool SCPipeBuilder::EmptySig(){
  bool ret = true;

  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    if( !SigMap->GetSignal(i)->IsPipeDefined() ){
      ret = false;
      if( Opts->IsVerbose() ){
        this->PrintRawMsg("EmptySig: signal=" +
                          SigMap->GetSignal(i)->GetName() +
                          " from instruction=" +
                          SigMap->GetSignal(i)->GetInst() +
                          " has no associated pipe stage");
      }
    }
  }

  return ret;
}

bool SCPipeBuilder::HasIOSigs(std::string Pipe){
  bool ret = false;

  unsigned Idx = PipeToIdx(Pipe);
  SCSig *Sig = nullptr;

  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    if( AdjMat[Idx][i] == 1 ){
      Sig = SigMap->GetSignal(i);
      if( Sig->isRegSig() ||
          Sig->isMemSig() ||
          (Sig->GetType() == PC_INCR) ){
        ret = true;
      }
    }
  }

  return ret;
}

bool SCPipeBuilder::IsAdjacent(SCSig *Base, SCSig *New){
  if( Base->GetType() == New->GetType() ){
    return true;
  }else if( Base->isALUSig() && New->isALUSig() ){
    return true;
  }
  return false;
}

bool SCPipeBuilder::FitArith(){
  // Attempts to fit any arithmetic operations that are not currently asssigned
  // to a given pipe stage to a predefined stage

  // record the list of arithmetic pipeline stages that are not assigned
  std::vector<SCSig *> ArithOps;
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    SCSig *Sig = SigMap->GetSignal(i);

    //if( !Sig->isMemSig() && !Sig->isRegSig() && !Sig->IsPipeDefined() ){
      //if( Sig->GetType() != PC_INCR ){
    if( (Sig->isALUSig() || Sig->isMuxSig() || Sig->isBranchSig()) &&
        (!Sig->IsPipeDefined()) ){

      // now back fit the set of ALU register operations AREG_READ/AREG_WRITE
      ArithOps.push_back( Sig );
      //}
    }
  }

  std::vector<SCSig *> ArithSigVect;
  bool done = false;

  for( unsigned i=0; i<ArithOps.size(); i++ ){
    if( Opts->IsVerbose() ){
      this->PrintRawMsg("FitArith: Fitting " + ArithOps[i]->GetName() +
                        " from instruction=" +  ArithOps[i]->GetInst() );
    }

    done = false;

    // Stage 1: Search local instruction for an adjacent arithmetic signal
    ArithSigVect = SigMap->GetSigVect(ArithOps[i]->GetInst());
    for( unsigned j=0; j<ArithSigVect.size(); j++ ){
      if( (ArithSigVect[j]->isALUSig()||ArithSigVect[j]->isMuxSig()||ArithSigVect[j]->isBranchSig()) &&
          (ArithSigVect[j]->IsPipeDefined()) &&
          (ArithOps[i] != ArithSigVect[j]) &&
          (!done) ){
        if( IsAdjacent(ArithOps[i],ArithSigVect[j]) ){
          // found a match
          AdjMat[PipeToIdx(ArithSigVect[j]->GetPipeName())][SigToIdx(ArithOps[i])] = 1;
          done = true;
          if( Opts->IsVerbose() ){
            this->PrintRawMsg("FitArith: Fitting " + ArithOps[i]->GetName() +
                              " to " + ArithSigVect[j]->GetPipeName());
          }
        }
      }
    }

    // Stage 2: Search for identical or adjacent signals across the remainder
    //          of all instructions
    if( !done ){
      SCSig *ASig = nullptr;
      for( unsigned j=0; j<SigMap->GetNumSignals(); j++ ){
        ASig = SigMap->GetSignal(j);
        if( (ASig->isALUSig()||ASig->isMuxSig()||ASig->isBranchSig()) &&
            (ASig != ArithOps[i]) &&
            (ASig->IsPipeDefined()) &&
            (IsAdjacent(ArithOps[i],ASig)) &&
            (!done) ){
          // found a match
          AdjMat[PipeToIdx(ASig->GetPipeName())][SigToIdx(ArithOps[i])] = 1;
          done = true;
          if( Opts->IsVerbose() ){
            this->PrintRawMsg("FitArith: Fitting " + ArithOps[i]->GetName() +
                              " to " + ASig->GetPipeName());
          }
        }
      }
    }

    // Stage 3: Fall back and find an arith stage that contains no I/O signals
    if( !done ){
      // find a pipe stage that isn't utilized by I/O signals
      std::vector<unsigned> PipeSums;
      unsigned Sum = 0;
      for( unsigned j=0; j<PipeVect.size(); j++ ){
        Sum = 0;
        for( unsigned k=0; k<SigMap->GetNumSignals(); k++ ){
          Sum = Sum + AdjMat[j][k];
        }
        PipeSums.push_back(Sum);
      }

      // first, try to fina an unused pipe stage
      for( unsigned j=0; j<PipeSums.size(); j++ ){
        if( (PipeSums[j] == 0) && (!done) ){
          AdjMat[j][SigToIdx(ArithOps[i])] = 1;
          done = true;
          if( Opts->IsVerbose() ){
            this->PrintRawMsg("FitArith: Fitting " + ArithOps[i]->GetName() +
                              " to " + PipeVect[j]);
          }
        }
      }

      if( !done ){
        // find an arith stage with no i/o signals
        unsigned j = 0;
        do{
          if( !HasIOSigs(PipeVect[j]) ){
            AdjMat[j][SigToIdx(ArithOps[i])] = 1;
            done = true;
            if( Opts->IsVerbose() ){
              this->PrintRawMsg("FitArith: Fitting " + ArithOps[i]->GetName() +
                                " to " + PipeVect[j]);
            }
          }
          j = j+1;
        }while( (j<PipeVect.size()) && (!done) );

#if 0
        for( unsigned i=0; i<PipeVect.size(); i++ ){
          if( (!HasIOSigs(PipeVect[i])) && (!done) ){
            AdjMat[i][SigToIdx(ArithOps[i])] = 1;
            done = true;
            if( Opts->IsVerbose() ){
              this->PrintRawMsg("FitArith: Fitting " + ArithOps[i]->GetName() +
                                " to " + PipeVect[i]);
            }
          }
        }
#endif
      }
    }

    // print failure
    if( !done ){
      if( Opts->IsVerbose() ){
        this->PrintRawMsg("FitArith: Failed to fit " + ArithOps[i]->GetName());
      }
      return false;
    }

    // write the signal map back out
    if( !WriteSigMap() ){
      this->PrintMsg( L_ERROR, "Failed to write signal map during the FitArith phase" ); 
      return false;
    }
  }

  return true;
}

bool SCPipeBuilder::DeadPipeElim(){
  // Attempts to remove any pipeline stages that aren't involved in any operations

  std::vector<std::string> EmptyStages;
  for( unsigned i=0; i<PipeVect.size(); i++ ){
    unsigned Total = 0;
    for( unsigned j=0; j<SigMap->GetNumSignals(); j++ ){
      Total = Total + AdjMat[i][j];
    }
    if( Total == 0 )
      EmptyStages.push_back( PipeVect[i] );
  }

  if( EmptyStages.size() > 0 ){
    if( !FreeMat() ){
      this->PrintMsg( L_ERROR, "DeadPipeElim: Could not free adjacency matrix" );
      return false;
    }

    // delete each empty pipe
    for( unsigned i=0; i<EmptyStages.size(); i++ ){
      auto it = std::find( PipeVect.begin(), PipeVect.end(), EmptyStages[i] );
      if( Opts->IsVerbose() ){
        this->PrintRawMsg("DeadPipeElim: Removing dead pipe=" + EmptyStages[i] );
      }
      PipeVect.erase(it);
    }

    // allocate the new matrix and build it out
    if( !AllocMat() ){
      this->PrintMsg( L_ERROR, "DeepPipeElim: Could not allocate matrix" );
      return false;
    }

    // build the pipeline matrix
    if( !BuildMat() ){
      this->PrintMsg( L_ERROR, "DeepPipeElim: Could not build matrix representation of pipeline" );
      return false;
    }
  }

  return true;
}

bool SCPipeBuilder::SplitIO(){

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
      this->PrintMsg( L_ERROR, "SplitIO: Failed to allocate matrix" );
      return false;
    }

    if( Opts->IsVerbose() ){
      this->PrintRawMsg("SplitIO: No pipeline stages found, splitting into 5-stage pipeline");
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

  //
  // Phase 1: Mixed pipeline
  //
  // We have a mixture of currently defined pipeline states
  // We will need to derive which instructions have pipelines
  // containing register I/O operations and which do not.  For those
  // that do not, we need to "fit" them into the correct stages.

  // SplitPipes: signal_ptr:pipeline:pipe_stage
  std::vector<std::tuple<SCSig *,std::string,std::string>> SplitPipes;
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
          SplitPipes.push_back( std::tuple<SCSig *,std::string,std::string>(
                                  Sig,
                                  GetPipelineFromStage(IVect[j]->GetPipeName()),
                                  Sig->GetPipeName() ) );
        } // end conditional
      } // for IVect loop
    } // end conditional
  } // end signal loop

  // determine if we have any candidate pipe stages to adjust
  if( SplitPipes.size() > 0 ){
    std::vector<std::string> Unique;  // unique pipelines to split
    for( unsigned i=0; i<SplitPipes.size(); i++ ){
      Unique.push_back(std::get<1>(SplitPipes[i]));
    }

    // find the unique pipelines
    std::sort( Unique.begin(), Unique.end() );
    Unique.erase( std::unique(Unique.begin(),Unique.end()), Unique.end());

    // free the adjacency matrix
    if( !FreeMat() ){
      this->PrintMsg( L_ERROR, "SplitIO: Could not free adjacency matrix" );
      return false;
    }

    // records the mapping of Pipeline to PipeStage
    std::map<std::string,std::string> PipeMap;

    // split each pipeline's i/o stages into REG_READ, REG_WRITE and MEMORY
    for( unsigned i=0; i<Unique.size(); i++ ){
      // split Unique[i]: find the first candidate pipe to split
      bool done = false;
      unsigned j = 0;
      while( !done ){
        if( std::get<1>(SplitPipes[j]) == Unique[i] ){
          PipeVect.push_back( std::get<2>(SplitPipes[j]) + "_REG_READ" );
          PipeVect.push_back( std::get<2>(SplitPipes[j]) + "_REG_WRITE" );
          PipeVect.push_back( std::get<2>(SplitPipes[j]) + "_MEMORY" );
          PipeMap[Unique[i]] = std::get<2>(SplitPipes[j]); // record the pipe to pipe stage mapping
          done = true;
        }
        j += 1;
      }
    }

    // allocate the new matrix and build it out
    if( !AllocMat() ){
      this->PrintMsg( L_ERROR, "SplitIO: Could not allocate matrix" );
      return false;
    }

    // build the pipeline matrix
    if( !BuildMat() ){
      this->PrintMsg( L_ERROR, "SplitIO: Could not build matrix representation of pipeline" );
      return false;
    }

    // now that we have the signals identified, the new pipe stage constructed
    // and the adjacency matrix built,
    // walk through our list of identified signals and adjust the i/o's
    for( unsigned i=0; i<SplitPipes.size(); i++ ){
      SCSig *Sig            = std::get<0>(SplitPipes[i]);
      std::string Pipeline  = std::get<1>(SplitPipes[i]);
      std::string SigName   = Sig->GetName();
      std::string Stage     = PipeMap[Pipeline];
      unsigned SigIdx       = SigToIdx(Sig);

      // clear out all the existing mappings
      if( !ClearSignal(Sig) ){
        this->PrintMsg( L_ERROR, "SplitIO: Could not clear signal to pipe mapping for: " + SigName );
        return false;
      }

      if( Opts->IsVerbose() ){
        this->PrintRawMsg("SplitIO: Splitting I/O for signal=" +
                          SigName +
                          " from instruction=" +
                          Sig->GetInst() );
      }

      // set the new signals
      switch( Sig->GetType() ){
      case REG_READ:
        AdjMat[PipeToIdx(Stage+"_REG_READ")][SigIdx] = 1;
        break;
      case REG_WRITE:
        AdjMat[PipeToIdx(Stage+"_REG_WRITE")][SigIdx] = 1;
        break;
      case AREG_READ:
      case AREG_WRITE:
        AdjMat[PipeToIdx(Stage)][SigIdx] = 1;
        break;
      case MEM_READ:
      case MEM_WRITE:
        AdjMat[PipeToIdx(Stage+"_MEMORY")][SigIdx] = 1;
        break;
      default:
        this->PrintMsg( L_ERROR,
                        "SplitIO: Unknown adjustment for Signal=" +
                        SigName + " in instruction=" + Sig->GetInst() );
        return false;
        break;
      }
    }
  }

  // write all the intermediate results back to the core data structures
  if( !WriteSigMap() ){
    this->PrintMsg( L_ERROR, "Failed to write signal map during the SplitIO phase" ); 
    return false;
  }

  //
  // Phase 2: Mixed Pipeline
  // back fit any i/o stages that have no pipes defined
  //
  SCSig *Input = nullptr;
  SCSig *Fit   = nullptr;
  std::vector<SCSig *> SV;
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    Input = SigMap->GetSignal(i);
    if( (Input->isMemSig() || Input->isRegSig()) && (!Input->IsPipeDefined()) ){
      //
      // We found an I/O signal with no candidate pipe stage
      // First, search the signals in the same instruction and determine
      // whether there is a good fit for an I/O signal
      //
      // If not, search all the signals for a good fit for an I/O signal
      //
      bool done = false;

      // search the signals within the target instruction
      SV = SigMap->GetSigVect(Input->GetInst());
      for( unsigned j=0; j<SV.size(); j++ ){
        if( SV[j]->IsPipeDefined() &&
            (Input->GetType() == SV[j]->GetType()) &&
            (!done) ){
          done = true;
          AdjMat[PipeToIdx(SV[j]->GetPipeName())][i] = 1;
          if( Opts->IsVerbose() ){
            this->PrintRawMsg( "SplitIO: Fitting Signal=" +
                               Input->GetName() + " in instruction=" +
                               Input->GetInst() + " to pipe stage=" +
                               SV[j]->GetPipeName() );
          }
        }
      }

      // search all the signals
      if( !done ){
        for( unsigned j=0; j<SigMap->GetNumSignals(); j++ ){
          if( (SigMap->GetSignal(j) != Input) &&
              (SigMap->GetSignal(j)->IsPipeDefined()) &&
              (SigMap->GetSignal(j)->GetType() == Input->GetType()) &&
              (!done) ){
            done = true;
            Fit = SigMap->GetSignal(j);
            AdjMat[PipeToIdx(Fit->GetPipeName())][i] = 1;
            if( Opts->IsVerbose() ){
              this->PrintRawMsg( "SplitIO: Fitting Signal=" +
                                Input->GetName() + " in instruction=" +
                                Input->GetInst() + " to pipe stage=" +
                                Fit->GetPipeName() );
            }
          }
        }
      }
    }
  }// end back fit loop

  //
  // Phase 2: Mixed Pipeline
  // now back fit the set of ALU register operations AREG_READ/AREG_WRITE
  //

  return true;
}

bool SCPipeBuilder::ClearSignal(SCSig *S){
  unsigned SigIdx = SigToIdx(S);

  if( SigIdx == SigMap->GetNumSignals() )
    return false;

  for( unsigned i=0; i<PipeVect.size(); i++ ){
    AdjMat[i][SigIdx] = 0;
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

unsigned SCPipeBuilder::SigToIdx(SCSig *Sig){
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    if( SigMap->GetSignal(i) == Sig)
      return i;
  }
  return SigMap->GetNumSignals();
}

bool SCPipeBuilder::Optimize(){
  bool (SCPipeBuilder::*Pass)() = nullptr;

  for( unsigned i=0; i<Enabled.size(); i++ ){
    Pass = Enabled[i].second;
    if( Opts->IsVerbose() ){
      this->PrintRawMsg("Executing subpass: " + Enabled[i].first );
    }
    if( !(*this.*Pass)() ){
      this->PrintMsg( L_ERROR, "Subpass failed: " + Enabled[i].first );
      return false;
    }
    if( !WriteSigMap() ){
      this->PrintMsg( L_ERROR, "Failed to write signal map following " + Enabled[i].first );
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

  AdjMat = nullptr;

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
  Enabled.push_back(std::make_pair("SplitIO",
                                   &SCPipeBuilder::SplitIO) );
  Enabled.push_back(std::make_pair("FitArith",
                                   &SCPipeBuilder::FitArith) );
  Enabled.push_back(std::make_pair("DeadPipeElim",
                                   &SCPipeBuilder::DeadPipeElim) );
  Enabled.push_back(std::make_pair("EmptySig",
                                   &SCPipeBuilder::EmptySig) );
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
