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
  : SCPass("VLIWPipeBuilder","",TM,O,M) {
}

SCVLIWPipeBuilder::~SCVLIWPipeBuilder(){
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
          }
        }
      }
    }
  }
  return true;
}

bool SCVLIWPipeBuilder::WireUpStages(){
  VLIWGraph *Graph = new VLIWGraph();

  // insert all the stages into the graph w/o edges
  for( unsigned i=0; i<VLIWStages.size(); i++ ){
    VLIWNode *N = new VLIWNode(VLIWStages[i].first,
                               VLIWStages[i].second);
    Graph->InsertNode(N);
  }

  // walk the vliw stages in order.  wire up the necessary
  // output I/O signals to the subsequent stages
  if( !WireIO(Graph) ){
    delete Graph;
    return false;
  }

  delete Graph;
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
    if( IsVLIWStage(Func) ){
      // found a vliw function
      VLIWStages.push_back(std::make_pair(Func.getName(),
                                          GetVLIWStage(Func)));
    }
  }

  // sort the vector by the stage (.second)
  std::sort(VLIWStages.begin(), VLIWStages.end(), SortBySec);

  return true;
}

bool SCVLIWPipeBuilder::EnableSubPasses(){
  if( Opts->IsVerbose() )
    this->PrintRawMsg("Enabling VLIW Pipeline Passes");

  Enabled.push_back(std::make_pair("DiscoverPipeSlots",
                                   &SCVLIWPipeBuilder::DiscoverPipeSlots));
  Enabled.push_back(std::make_pair("WireUpStages",
                                   &SCVLIWPipeBuilder::WireUpStages));
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
    if( !WriteSigMap() ){
      this->PrintMsg( L_ERROR, "Failed to write signal map following " +
                      Enabled[i].first);
      return false;
    }
  }

  return true;
}

bool SCVLIWPipeBuilder::WriteSigMap(){
  return true;
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

  //
  // Notes from JDL:
  //
  // There are a number of potential paths here.
  //
  // First, there are two paths for VLIW:
  //
  // 1. specify the bundle types in the CoreGen+StoneCutter files (not yet done)
  //
  // 2. utilize the StoneCutter functions defined as 'VLIW' with the In/Out
  //    intrinsics in order to determine how the pipelines are wired up.
  //
  // We should make a decision whether you can mix these concepts.
  // For (1), we should be able to quickly determine whether the bundle
  // specifications are there.  In which case, we can effectively utilize
  // the pipeline logic generated by the SCPipeBuilder to handle the
  // normal pipelines and the SCVLIWPipeBuilder to wire the top-level
  // pipeline together (crack+decode; dispatch)
  //
  // For (2), we'll need to first use the function names to wire up
  // how the pipeline(s) are connected STAGE_x where `x1 defines the
  // stage number (monotonically increasing).  Then we can utilize the
  // In/Out intrinsics to determine "how" they're wired together
  // (control/data paths).  If we find functions w/o In/Out intrinsics, we'll
  // need to try and derive how they're wired up.  If we can't derive them, then
  // we'll need to error out
  //
  //
  // We don't necessarily need to utilize the same methods employed in the
  // existing SCPipeBuilder.
  //

#if 0
  // TODO: Not sure if InstVect should use the same as PipeBuilder... seems like a BundleParser that would look at the sigmap may be more appropriate
  VLIWPipeVect = SigMap->GetVLIWPipeVect();
  // BundleVect = SigMap->GetBundleVect();
  // InstVect = ParseBundles();

  // Enable SubPasses (TODO: VLIW Specific?)
  if( !EnableSubPasses() ) {
    this->PrintMsg( L_ERROR, "Could not initialize VLIW sub-passes");
    return false;
  }
  // Initalize the attribute map (TODO: This would remain the same process just different attributes... I think)
  if( !InitAttrs() ){
    this->PrintMsg( L_ERROR, "Could not initialize attributes" );
    return false;
  }

  // TODO: Would there be a matrix? VLIW Payload won't have crack & decode yet
  // allocate the matrix
  if( !AllocMat() ){
    this->PrintMsg( L_ERROR, "Could not allocate matrix" );
    return false;
  }

  // build the pipeline matrix (TODO: This should be replaced by a bundle scheduler... is In/Out the only constraints a user can specify?)
  if( !BuildMat() ){
    this->PrintMsg( L_ERROR, "Could not build matrix representation of pipeline" );
    FreeMat();
    return false;
  }

  // execute optimizations (TODO: We should be able to use this as is w/ different passes enabled of course)
  if( !Optimize() ){
    this->PrintMsg( L_ERROR, "Encountered errors executing the the optimization phases" );
    FreeMat();
    return false;
  }

  // write everything back to the sigmap (TODO: This should be able to be used as well, just maybe specifying which pipeline to write back to?)
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

#endif
  return true;
}

// EOF

