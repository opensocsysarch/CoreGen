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
  }else if( Base->isMemSig() && New->isMemSig() ){
    return true;
  }else if( Base->isMuxSig() && New->isMuxSig() ){
    return true;
  }else if( Base->isBranchSig() && New->isBranchSig() ){
    return true;
  }
  return false;
}

bool SCPipeBuilder::FitPCSigs(){

  //
  // Stage 1: Fit all the PC_INCR signals
  //
  // This requires two steps as follows:
  // First, look for any PC_INCR signals that are already allocated
  // If we find a candidate stage, then fit all the other PC_INCR signals into
  // this stage.  If we don't find a candidate stage, then split a new stage off
  // called "FETCH" and fit all the PC_INCR stages here.
  //
  std::string FetchStage;
  bool done = false;
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    if( (SigMap->GetSignal(i)->GetType() == REG_WRITE) &&
        (SigMap->GetSignal(i)->IsPipeDefined()) &&
        (!done)){
      FetchStage = SigMap->GetSignal(i)->GetPipeName();
      done = true;
    }
  }

  if( FetchStage.length() == 0 ){
    // we must split off a new stage
    FetchStage = "WRITE_BACK";
    auto it = PipeVect.begin();
    it = PipeVect.insert(it,FetchStage);

    // free the existing matrix
    if( !FreeMat() ){
      this->PrintMsg( L_ERROR, "FitPCSigs: Could not free adjacency matrix" );
      return false;
    }

    // allocate the new matrix and build it out
    if( !AllocMat() ){
      this->PrintMsg( L_ERROR, "FitPCSigs: Could not allocate matrix" );
      return false;
    }

    // build the pipeline matrix
    if( !BuildMat() ){
      this->PrintMsg( L_ERROR, "FitPCSigs: Could not build matrix representation of pipeline" );
      return false;
    }
  }

  // now fit all the existing PC_INCR signals to "FetchStage"
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    if( (SigMap->GetSignal(i)->isPCSig() ) &&
        (!SigMap->GetSignal(i)->IsPipeDefined()) ){
      if( Opts->IsVerbose() ){
        this->PrintRawMsg( "FitPCSigs: Fitting " + SigMap->GetSignal(i)->GetName() +
                           " from instruction = " +
                           SigMap->GetSignal(i)->GetInst() + " to " + FetchStage );
      }
      AdjMat[PipeToIdx(FetchStage)][i] = 1;
    }
  }

  return true;
}

bool  SCPipeBuilder::FitTmpReg(){

  SCSig *Sig = nullptr;
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    Sig = SigMap->GetSignal(i);
    if( ((Sig->GetType() == AREG_READ) ||
        (Sig->GetType() == AREG_WRITE)) &&
        (!Sig->IsPipeDefined())){
      // found a candidate temp signal
      std::vector<SCSig *> SV = SigMap->GetSigVect(Sig->GetInst());

      for( unsigned j=0; j<SV.size(); j++ ){
        if( ((SV[j]->isALUSig()) ||
            (SV[j]->isMuxSig()) ||
            (SV[j]->isBranchSig())) &&
            SV[j]->IsPipeDefined() ){
          AdjMat[PipeToIdx(SV[j]->GetPipeName())][i] = 1;
          if( Opts->IsVerbose() ){
            this->PrintRawMsg("FitTmpReg: Fitting " + Sig->GetName() +
                              " to " + SV[j]->GetPipeName());
          }
        }
      }
    }
  }

  return true;
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
        for( unsigned j=0; j<PipeVect.size(); j++ ){
          if( (!HasIOSigs(PipeVect[j])) && (!done) ){
            AdjMat[j][SigToIdx(ArithOps[i])] = 1;
            done = true;
            if( Opts->IsVerbose() ){
              this->PrintRawMsg("FitArith: Fitting " + ArithOps[i]->GetName() +
                                " to " + PipeVect[j]);
            }
          }
        }
      }
    }

    // print failure
    if( !done ){
      if( Opts->IsVerbose() ){
        this->PrintRawMsg("FitArith: Failed to fit " + ArithOps[i]->GetName());
      }
      return false;
    }
  }

  return true;
}

void SCPipeBuilder::PrintAdjMat(){
  for( unsigned i=0; i<PipeVect.size(); i++ ){
    std::cout << "[" << PipeVect[i] << "] ";
    for( unsigned j=0; j<SigMap->GetNumSignals(); j++ ){
      std::cout << "[" << AdjMat[i][j] << "] ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

std::vector<std::string> SCPipeBuilder::GetEmptyStages(){

  std::vector<std::string> EmptyStages;
  for( unsigned i=0; i<PipeVect.size(); i++ ){
    unsigned Total = 0;
    for( unsigned j=0; j<SigMap->GetNumSignals(); j++ ){
      Total = Total + AdjMat[i][j];
    }
    if( Total == 0 ){
      std::cout << "!!!!!!!!!!!! " << PipeVect[i] << std::endl;
      EmptyStages.push_back( PipeVect[i] );
    }
  }

  return EmptyStages;
}

bool SCPipeBuilder::DeadPipeElim(){
  // Attempts to remove any pipeline stages that aren't involved in any operations
  std::vector<std::string> EmptyStages = GetEmptyStages();

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

bool SCPipeBuilder::SplitNStage(){
  // First determine whether the user has prescribed us to setup an N-stage pipeline
  std::map<std::string,unsigned> NStages;
  std::size_t found;
  unsigned stages;
  for( unsigned i=0; i<AttrMap.size(); i++ ){
    found = AttrMap[i].second.find("stages_");
    if( found != std::string::npos ){
      // split out the number of pipe stages
      stages = std::stoull( AttrMap[i].second.substr(found+7), 0, 10 );
      if( stages == 0 ){
        this->PrintMsg( L_ERROR,
                        "SplitNStage: 0 length pipeline stages are not permitted for pipeline=" +
                        AttrMap[i].first);
        return false;
      }
      NStages.insert( std::pair<std::string,unsigned>(AttrMap[i].first,stages) );
    }
  }

  // print the options
  if( Opts->IsVerbose() ){
    std::map<std::string,unsigned>::iterator it = NStages.begin();
    while( it != NStages.end() ){
      this->PrintRawMsg("SplitNStage: Pipeline " + it->first +
                        " prescribed to use " + std::to_string(it->second) +
                        " stages");
      it++;
    }
  }

  // determine whether each pipeline has sufficient stages defined
  // if not, split the pipeline into N stages
  std::map<std::string,unsigned>::iterator it = NStages.begin();
  std::vector<std::string> Stages;
  while( it != NStages.end() ){
    Stages = GetPipelineStages(it->first);
    if( Opts->IsVerbose() ){
      this->PrintRawMsg("SplitNStage: Pipeline " + it->first + " has " +
                        std::to_string(Stages.size())  + " predifined stages of " +
                        std::to_string(it->second) +
                        " prescribed");
    }

    if( it->second != Stages.size() ){
      // split the stages to N stages
      unsigned Base = 1;
      if( Stages.size() > 0 )
        Base = Stages.size();

      if( !FreeMat() ){
        this->PrintMsg( L_ERROR, "SplitNStage: Could not free adjacency matrix" );
        return false;
      }

      if( Opts->IsVerbose() ){
        this->PrintRawMsg("SplitNStage: Adding " +
                          std::to_string(it->second-Stages.size()) +
                          " stages for Pipeline=" + it->first );
      }

      for( unsigned i=0; i<(it->second-Stages.size()); i++ ){
        PipeVect.push_back( it->first + std::to_string(Base+i) );
        ExtStage.push_back( std::pair<std::string,std::string>( it->first,
                                                                it->first + std::to_string(Base+i)));
      }

      // allocate the new matrix and build it out
      if( !AllocMat() ){
        this->PrintMsg( L_ERROR, "SplitNStage: Could not allocate matrix" );
        return false;
      }

      // build the pipeline matrix
      if( !BuildMat() ){
        this->PrintMsg( L_ERROR, "SplitNStage: Could not build matrix representation of pipeline" );
        return false;
      }

    }

    it++;
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
    PipeVect.push_back("WRITE_BACK");
    PipeVect.push_back("MEMORY");

    if( GetPipelines().size() == 0 ){
      AttrMap.push_back( std::make_pair("default","stages_5") );
      AttrMap.push_back( std::make_pair("default","in_order") );
    }

    // add the new stages to the extension vector
    ExtStage.push_back(std::pair<std::string,std::string>(DerivePipeline("FETCH"),
                                                            "FETCH"));
    ExtStage.push_back(std::pair<std::string,std::string>(DerivePipeline("REG_READ"),
                                                            "REG_READ"));
    ExtStage.push_back(std::pair<std::string,std::string>(DerivePipeline("ARITH"),
                                                            "ARITH"));
    ExtStage.push_back(std::pair<std::string,std::string>(DerivePipeline("WRITE_BACK"),
                                                            "WRITE_BACK"));
    ExtStage.push_back(std::pair<std::string,std::string>(DerivePipeline("MEMORY"),
                                                            "MEMORY"));

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
          Idx = PipeToIdx("WRITE_BACK");
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
            (IVect[j]->isALUSig() || IVect[j]->isBranchSig() || IVect[j]->isMuxSig()) &&
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

    // retrieve the set of empty stages
    std::vector<std::string> EmptyStages = GetEmptyStages();

    // free the adjacency matrix
    if( !FreeMat() ){
      this->PrintMsg( L_ERROR, "SplitIO: Could not free adjacency matrix" );
      return false;
    }

    // records the mapping of Pipeline to PipeStages [read:write:memory:arith]
    std::map<std::string,std::tuple<std::string,std::string,std::string>> PipeMap;
    std::map<std::string,std::string> PipeArithMap;

    // split each pipeline's i/o stages into REAG_READ, WRITE_BACK and MEMORY
    // utilize the empty stages first before creating new stages
    unsigned Empty = 0;
    std::string READStage;
    std::string WRITEStage;
    std::string MEMStage;
    for( unsigned i=0; i<Unique.size(); i++ ){
      // split Unique[i]: find the first candidate pipe to split
      bool done = false;
      unsigned j = 0;
      while( !done ){
        if( std::get<1>(SplitPipes[j]) == Unique[i] ){
          // read stage
          if( Empty < EmptyStages.size() ){
            READStage = EmptyStages[Empty];
            Empty += 1;
          }else{
            // add a new pipe stage
            READStage = std::get<2>(SplitPipes[j]) + "_REG_READ";
            PipeVect.push_back( READStage );
            ExtStage.push_back(std::pair<std::string,std::string>(
                std::get<1>(SplitPipes[j]),READStage));
          }

          // write stage
          if( Empty < EmptyStages.size() ){
            WRITEStage = EmptyStages[Empty];
            Empty += 1;
          }else{
            // add a new pipe stage
            WRITEStage = std::get<2>(SplitPipes[j]) + "_WRITE_BACK";
            PipeVect.push_back( WRITEStage );
            ExtStage.push_back(std::pair<std::string,std::string>(
                std::get<1>(SplitPipes[j]),WRITEStage));
          }

          // memory stage
          if( Empty < EmptyStages.size() ){
            MEMStage = EmptyStages[Empty];
            Empty += 1;
          }else{
            // add a new pipe stage
            MEMStage = std::get<2>(SplitPipes[j]) + "_MEMORY";
            PipeVect.push_back( MEMStage );
            ExtStage.push_back(std::pair<std::string,std::string>(
                std::get<1>(SplitPipes[j]),MEMStage));
          }

          PipeMap.emplace(Unique[i],
                          std::make_tuple(READStage,WRITEStage,MEMStage));
          PipeArithMap[Unique[i]] = std::get<2>(SplitPipes[j]);
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
      std::tuple<std::string,std::string,std::string> Stage = PipeMap[Pipeline];
      std::string ArithStage = PipeArithMap[Pipeline];
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
        AdjMat[PipeToIdx(std::get<0>(Stage))][SigIdx] = 1;
        break;
      case REG_WRITE:
        AdjMat[PipeToIdx(std::get<1>(Stage))][SigIdx] = 1;
        break;
      case AREG_READ:
      case AREG_WRITE:
        AdjMat[PipeToIdx(ArithStage)][SigIdx] = 1;
        break;
      case MEM_READ:
      case MEM_WRITE:
        AdjMat[PipeToIdx(std::get<2>(Stage))][SigIdx] = 1;
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
            (IsAdjacent(Input,SV[j])) &&
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
              (IsAdjacent(Input,SigMap->GetSignal(j))) &&
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

#if 0
    PrintAdjMat();
#endif

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

std::string SCPipeBuilder::DerivePipeline(std::string Stage){
  std::string Pipe;
  std::vector<std::string> Pipelines = GetPipelines();

  if( Pipelines.size() == 0 ){
    Pipe = "default";
    return Pipe;
  }

  // stage 1: see if there is only one pipeline defined
  if( Pipelines.size() == 1 ){
    Pipe = Pipelines[0];
    return Pipe;
  }

  // stage 2: see if it was defined in the StoneCutter source
  for( unsigned i=0; i<Pipelines.size(); i++ ){
    std::vector<std::string> SV = GetPipelineStages(Pipelines[i]);
    for( unsigned j=0; j<SV.size(); j++ ){
      if( SV[j] == Stage )
        return Pipelines[i];
    }
  }

  // stage 3: scan the ExtStage vector for matches
  for( unsigned i=0; i<ExtStage.size(); i++ ){
    if( ExtStage[i].second == Stage )
      return ExtStage[i].first;
  }

  return Pipe;
}

bool SCPipeBuilder::WriteSigMap(){
  // set all the pipe stages
  for( unsigned i=0; i<SigMap->GetNumSignals(); i++ ){
    for( unsigned j=0; j<PipeVect.size(); j++ ){
      if( AdjMat[j][i] == 1 ){
        SigMap->GetSignal(i)->SetPipeName(PipeVect[j]);
      }
    }
  }

  // if no original pipelines were defined, then define one
  if( GetNumPipelines() == 0 ){
    SigMap->InsertPipeline("default");
  }

  // set the pipeline data
  for( unsigned i=0; i<PipeVect.size(); i++ ){
    SigMap->InsertPipelineStage(DerivePipeline(PipeVect[i]),
                                PipeVect[i]);
  }

  // set the attributes
  for( unsigned i=0; i<AttrMap.size(); i++ ){
    SigMap->InsertPipelineAttr(AttrMap[i].first,AttrMap[i].second);
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

  std::vector<std::string> Pipes = GetPipelines();
  for( unsigned i=0; i<Pipes.size(); i++ ){
    std::vector<std::string> PV = GetPipelineAttrs(Pipes[i]);
    SigMap->InsertPipeline(Pipes[i]);
    for( unsigned j=0; j<PV.size(); j++ ){
      AttrMap.push_back( std::make_pair(Pipes[i],PV[j]) );
    }
  }

  if( Opts->IsVerbose() ){
    for( unsigned i=0; i<AttrMap.size(); i++ ){
      this->PrintRawMsg("PipelineAttr: " +
                        AttrMap[i].first + ":" +
                        AttrMap[i].second );
    }
  }
  return true;
}

bool SCPipeBuilder::EnableSubPasses(){
  // temporarily enable all the sub-passes
  Enabled.push_back(std::make_pair("SplitNStage",
                                   &SCPipeBuilder::SplitNStage) );
  Enabled.push_back(std::make_pair("SplitIO",
                                   &SCPipeBuilder::SplitIO) );
  Enabled.push_back(std::make_pair("FitArith",
                                   &SCPipeBuilder::FitArith) );
  Enabled.push_back(std::make_pair("FitTmpReg",
                                   &SCPipeBuilder::FitTmpReg) );
  Enabled.push_back(std::make_pair("FitPCSigs",
                                   &SCPipeBuilder::FitPCSigs) );
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
