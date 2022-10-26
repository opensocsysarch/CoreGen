//
// _SCVLIWPipeBuilder_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCVLIWPipeBuilder.h"
#include "CoreGen/StoneCutter/SCMsg.h"

SCVLIWPipeBuilder::SCVLIWPipeBuilder(Module *TM,
                     SCOpts *O,
                     SCMsg *M)
  : SCPass("VLIWPipeBuilder","",TM,O,M), SigMap(nullptr), Graph(nullptr) {
}

SCVLIWPipeBuilder::~SCVLIWPipeBuilder(){
  if( Graph )
    delete Graph;
}

void SCVLIWPipeBuilder::InsertSignalEdge(VLIWGraph *Graph,
                                         std::string SrcFunc,
                                         std::string DestFunc,
                                         unsigned SrcStage,
                                         unsigned DestStage,
                                         std::string Arg,
                                         std::string Type,
                                         unsigned Width){
  // check to see if SrcFunc->DestFunc has an edge.
  // If not, create one
  VLIWNode *SrcNode = Graph->GetNodeByName(SrcFunc, SrcStage);
  VLIWNode *DestNode = Graph->GetNodeByName(DestFunc, DestStage);

  VLIWEdge *Edge = nullptr;
  for( unsigned i=0; i<SrcNode->GetNumEdges(); i++ ){
    VLIWEdge *E = SrcNode->GetEdge(i);
    if( (E->GetSrc() == SrcFunc) &&
        (E->GetDest() == DestFunc) ){
      Edge = E;
    }
  }

  if( !Edge ){
    Edge = new VLIWEdge( SrcFunc, DestFunc );
    SrcNode->InsertEdge(Edge);
    DestNode->InsertEdge(Edge);
  }

  // create the signal
  VLIWSig *OutSig = nullptr;
  VLIWSig *InSig  = nullptr;
  if( (Type=="DATA") || (Type=="VAR") ){
    OutSig = new VLIWSig(Arg,
                         Width,
                         DATA_OUT);
    InSig = new VLIWSig(Arg,
                        Width,
                        DATA_IN);
  }else{
    OutSig = new VLIWSig(Arg,
                         Width,
                         CTRL_OUT);
    InSig = new VLIWSig(Arg,
                        Width,
                        CTRL_IN);
  }
  Edge->InsertSignal(OutSig);
  Edge->InsertSignal(InSig);
}

bool SCVLIWPipeBuilder::WireIO(VLIWGraph *Graph){
  for( unsigned i=0; i<VLIWStages.size(); i++ ){
    Function *Func = TheModule->getFunction(StringRef(VLIWStages[i].first));
    if( !Func ){
      this->PrintMsg( L_ERROR, "Failed to find VLIW function in the symbol table: " +
                      VLIWStages[i].first);
      return false;
    }

    for( auto &BB : Func->getBasicBlockList() ){
      for( auto &Inst : BB.getInstList() ){
        if( auto *CInst = dyn_cast<CallInst>(&Inst) ){
          if( CInst->getCalledFunction()->getName().str() ==
              "OUT" ){
            // For each 'OUT' intrinsic, generate an edge and insert it into
            // the appropriate nodes
            // This requires us to for N+1 stages that include the appropriately
            // named input arguments
            bool FoundNextStage = false;
            std::string Arg = GetVLIWArgName(Inst);
            unsigned Width = GetVLIWArgWidth(Inst);
            std::string Type = GetVLIWArgType(Inst);

            for( unsigned j=0; j<VLIWStages.size(); j++ ){
              if( VLIWStages[j].second >= VLIWStages[i].second ){
              //if( (VLIWStages[j].second == (VLIWStages[i].second+1)) ||
              //    (VLIWStages[j].second == (VLIWStages[i].second)) ){
                // Nth+1 stage found
                // Walk the target arguments and check for matches
                // If a match is found, insert an edge
                Function *TFunc = TheModule->getFunction(
                  StringRef(VLIWStages[j].first));
                if( !TFunc ){
                  this->PrintMsg( L_ERROR,
                                  "Failed to retrieve function pointer for VLIWStage: " +
                                  VLIWStages[j].first);
                  return false;
                }
                for( auto FArg = TFunc->arg_begin();
                      FArg != TFunc->arg_end();
                      ++FArg ){
                  if(FArg->getName().str() == Arg){
                    // found a matching argument, add signal into an edge
                    FoundNextStage = true;
                    InsertSignalEdge(Graph,
                                     VLIWStages[i].first,
                                     VLIWStages[j].first,
                                     VLIWStages[i].second,
                                     VLIWStages[j].second,
                                     Arg,
                                     Type,
                                     Width);
                  }
                }
              }
            }
            if( !FoundNextStage ){
              this->PrintMsg( L_ERROR,
                              "Failed to discover a downstream VLIW pipeline stage with an appropriate input; Stage=" +
                              VLIWStages[i].first + "; Argument=" + Arg );
            }
          }
        }
      }
    }
  }
  return true;
}

bool SCVLIWPipeBuilder::WireUpStages(){
  Graph = new VLIWGraph();

  // insert all the stages into the graph w/o edges
  for( unsigned i=0; i<VLIWStages.size(); i++ ){
    Graph->InsertNode( new VLIWNode(VLIWStages[i].first,
                           VLIWStages[i].second) );
  }

  // walk the vliw stages in order.  wire up the necessary
  // output I/O signals to the subsequent stages
  if( !WireIO(Graph) )
    return false;

  return true;
}

bool SCVLIWPipeBuilder::DiscoverPipeSlots(){
  std::vector<unsigned> Slots;

  // build a vector of slot numbers
  for( unsigned i=0; i<VLIWStages.size(); i++ ){
    Slots.push_back(VLIWStages[i].second);
  }

  // sort and make the vector unique
  std::sort(Slots.begin(), Slots.end());
  Slots.erase( std::unique(Slots.begin(),Slots.end()), Slots.end());

  // walk all the unique slots and look for monotonically increasing values
  for( unsigned i=1; i<Slots.size(); i++ ){
    if( Slots[i] != (Slots[i-1]+1) ){
      this->PrintMsg( L_ERROR, "Found a gap in slot numbering between " +
                              std::to_string(Slots[i-1]) + " and " +
                              std::to_string(Slots[i]) );
      return false;
    }
  }

  return true;
}

bool SortBySec(const std::pair<std::string,unsigned> &a,
               const std::pair<std::string,unsigned> &b){
  return (a.second < b.second);
}

bool SCVLIWPipeBuilder::DeriveVLIWStages(){
  for( auto &Func : TheModule->getFunctionList() ){
    std::cout << "looking at : " << Func.getName().str() << std::endl;
    if( IsVLIWStage(Func) ){
      // found a vliw function
      VLIWStages.push_back(std::make_pair(Func.getName(),
                                          GetVLIWStage(Func)));
    }
  }

  // sort the vector by the stage (.second)
  std::sort(VLIWStages.begin(), VLIWStages.end(), SortBySec);

  for( unsigned i=0; i<VLIWStages.size(); i++ ){
    std::cout << "VLIW_STAGE = " << VLIWStages[i].first << ":"
              << VLIWStages[i].second << std::endl;
  }

  return true;
}

bool SCVLIWPipeBuilder::CheckSignalIntegrity(){
  // Walk the graph object and ensure that all the OUT signals
  // have complementary IN signals.
  //
  // Also check to ensure that the incoming function arguments
  // are accounted for with IN statements.  If not, flag a warning
  for( unsigned i=0; i<VLIWStages.size(); i++ ){
    Function *Func = TheModule->getFunction(StringRef(VLIWStages[i].first));
    if( !Func ){
      this->PrintMsg( L_ERROR, "Failed to find VLIW function in the symbol table: " +
                      VLIWStages[i].first);
      return false;
    }

    unsigned Out = 0;
    unsigned In  = 0;
    for( auto &BB : Func->getBasicBlockList() ){
      for( auto &Inst : BB.getInstList() ){
        if( auto *CInst = dyn_cast<CallInst>(&Inst) ){
          if( CInst->getCalledFunction()->getName().str() ==
              "OUT" ){
            Out++;
          }else if( CInst->getCalledFunction()->getName().str() ==
                    "IN" ){
            In++;
          }
        }
      }
    }

    // check that the number of args matches the number of IN intrinsics
    if( (unsigned)(Func->arg_size()) != In ){
      this->PrintMsg( L_ERROR,
                      "The number of arguments does not match the number of defined input signals for Stage=" +
                      VLIWStages[i].first);
      return false;
    }

    // now we want to walk the graph for each output output statement and ensure
    // that a complementary input statement somewhere else exists
    unsigned TOut = 0;
    unsigned TIn  = 0;
    VLIWNode *Node = Graph->GetNodeByName(VLIWStages[i].first,
                                          VLIWStages[i].second);
    for( unsigned j=0; j<Node->GetNumEdges(); j++ ){
      VLIWEdge *Edge = Node->GetEdge(j);
      for( unsigned k=0; k<Edge->GetNumSignals(); k++ ){
        VLIWSig *Sig = Edge->GetSignal(k);
        if( (Edge->GetSrc() == VLIWStages[i].first) &&
            (Sig->GetType() == DATA_OUT) ){
          TOut++;
        }else if( (Edge->GetDest() == VLIWStages[i].first) &&
                  (Sig->GetType() == DATA_IN) ){
          TIn++;
        }
      }
    }

    if( TOut != Out ){
      this->PrintMsg( L_ERROR,
                      "Unconnected outgoing signal for Stage=" +
                      VLIWStages[i].first);
      return false;
    }
    if( TIn != In ){
      this->PrintMsg( L_ERROR,
                      "Unconnected incoming signal for Stage=" +
                      VLIWStages[i].first);
      return false;
    }
  }

  return true;
}

bool SCVLIWPipeBuilder::EnableSubPasses(){
  if( Opts->IsVerbose() )
    this->PrintRawMsg("Enabling VLIW Pipeline Passes");

  Enabled.push_back(std::make_pair("DiscoverPipeSlots",
                                   &SCVLIWPipeBuilder::DiscoverPipeSlots));
  Enabled.push_back(std::make_pair("WireUpStages",
                                   &SCVLIWPipeBuilder::WireUpStages));
  Enabled.push_back(std::make_pair("CheckSignalIntegrity",
                                   &SCVLIWPipeBuilder::CheckSignalIntegrity));
  return true;
}

bool SCVLIWPipeBuilder::Optimize(){
  bool (SCVLIWPipeBuilder::*Pass)() = nullptr;

  for( unsigned i=0; i<Enabled.size(); i++ ){
    Pass = Enabled[i].second;
    if( Opts->IsVerbose() ){
      this->PrintRawMsg("Executing subpass: " + Enabled[i].first );
    }
    if( !(*this.*Pass)() ){
      this->PrintMsg( L_ERROR, "Subpass failed: " + Enabled[i].first );
      return false;
    }
  }

  return true;
}

bool SCVLIWPipeBuilder::WriteSigMap(){

  // walk all the nodes and inject the signals into
  // the appropriate VLIW stage
  for( unsigned i=0; i<Graph->GetNumNodes(); i++ ){
    for( unsigned j=0; j<Graph->GetNode(i)->GetNumEdges(); j++ ){
      for( unsigned k=0; k<Graph->GetNode(i)->GetEdge(j)->GetNumSignals(); k++ ){
        VLIWSig *VSig = Graph->GetNode(i)->GetEdge(j)->GetSignal(k);
        if( (VSig->GetType() == DATA_IN) ||
            (VSig->GetType() == CTRL_IN) ){
          if( Graph->GetNode(i)->GetName() ==
              Graph->GetNode(i)->GetEdge(j)->GetDest() ){
            // insert an input signal
            SCSig *Sig = new SCSig(VSig->GetType(),
                               VSig->GetWidth());
            Sig->InsertInput(VSig->GetName());
            Sig->SetInst(Graph->GetNode(i)->GetEdge(j)->GetDest());
            Sig->SetPipeName(Graph->GetNode(i)->GetEdge(j)->GetDest());
            Sig->SetName(Graph->GetNode(i)->GetEdge(j)->GetSrc() + "_" +
                         Graph->GetNode(i)->GetEdge(j)->GetDest() + "_" +
                         VSig->GetName());
            if( !SigMap->EmplaceSignal(Sig) ){
              this->PrintMsg(L_ERROR, "Error injecting IN signal into signal map");
              return false;
            }
          }
        }else if( (VSig->GetType() == DATA_OUT) ||
                  (VSig->GetType() == CTRL_OUT) ){
          if( Graph->GetNode(i)->GetName() ==
              Graph->GetNode(i)->GetEdge(j)->GetSrc() ){
            // insert an output signal
            SCSig *Sig = new SCSig(VSig->GetType(),
                               VSig->GetWidth());
            Sig->InsertInput(VSig->GetName());
            Sig->SetInst(Graph->GetNode(i)->GetEdge(j)->GetSrc());
            Sig->SetPipeName(Graph->GetNode(i)->GetEdge(j)->GetSrc());
            Sig->SetName(Graph->GetNode(i)->GetEdge(j)->GetSrc() + "_" +
                         Graph->GetNode(i)->GetEdge(j)->GetDest() + "_" +
                         VSig->GetName());
            if( !SigMap->InsertSignal(Sig) ){
              this->PrintMsg(L_ERROR, "Error injecting OUT signal into signal map");
              return false;
            }
          }
        }
      }
    }
  }

  return SigMap->WriteSigMap();
}

bool SCVLIWPipeBuilder::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // Retrieve the VLIW stages
  if( !DeriveVLIWStages() ){
    this->PrintMsg( L_ERROR, "Encountered errors deriving VLIW stages" );
    return false;
  }

  if( VLIWStages.size() == 0 ){
    // no stages to optimize, eject
    if( Opts->IsVerbose() ){
      this->PrintRawMsg("No VLIW stages found");
    }
    return true;
  }

  // Enable all the subpasses
  if( !EnableSubPasses() ){
    this->PrintMsg( L_ERROR, "Could not initialize sub-passes" );
    return false;
  }

  // Execute the optimizations/checkers
  if( !Optimize() ){
    this->PrintMsg( L_ERROR, "Encountered errors executing the the optimization phases" );
    return false;
  }

  // Write the signal map back out
  if( !WriteSigMap() ){
    return false;
  }

  return true;
}

// EOF

