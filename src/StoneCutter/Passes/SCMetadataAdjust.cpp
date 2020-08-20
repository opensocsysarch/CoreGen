//
// _SCMetadataAdjust_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCMetadataAdjust.h"

SCMetadataAdjust::SCMetadataAdjust(Module *TM,
                     SCOpts *O,
                     SCMsg *M)
  : SCPass("MetadataAdjust","",TM,O,M) {
}

SCMetadataAdjust::~SCMetadataAdjust(){
}

void SCMetadataAdjust::SetMetadata(BasicBlock &BB,
                                   unsigned Instruction,
                                   MDNode *NameNode,
                                   MDNode *LineNode){
  unsigned i = 0;
  for( auto &Inst : BB.getInstList() ){
    if( i == Instruction ){
      if( Opts->IsVerbose() ){
        this->PrintRawMsg("Adjusting " +
                          std::string(Inst.getOpcodeName()) +
                          " within " +
                          BB.getParent()->getName().str());
      }
      Inst.setMetadata("pipe.pipeName",NameNode);
      Inst.setMetadata("pipe.pipeLine",LineNode);
      return ;
    }
    i = i+1;
  }
}

void SCMetadataAdjust::ExamineBBMetadata(BasicBlock &BB){
  std::vector<std::string> PipeName;
  std::vector<std::string> PipeLine;

  // make sure we have enough work to do
  if( BB.size() < 3 )
    return ;

  std::string Tmp1;
  std::string Tmp2;
  // build up our instruction vectors
  for( auto &Inst : BB.getInstList() ){
    if( !GetPipeStage(Inst,Tmp1) )
      Tmp1 = ".";
    if( !GetPipeLine(Inst,Tmp2) )
      Tmp2 = ".";
    PipeName.push_back(Tmp1);
    PipeLine.push_back(Tmp2);
  }

  //
  // Walk the instruction vector in tuples
  // We need to examine the i'th and i'th+2
  // instructions and the associated metadata.
  // If the inner most instruction does not
  // have metadata and the outer (halo) instructions
  // contain identical metadata, then set the inner
  // instruction metadata to match the adjacent pair.
  // In this case, we retrieve the pipeName and pipeLine
  // for each instruction.
  //
  // Inst[i]   ==> !pipe.pipeName !0, !pipe.pipeLine !1, !pipe.pipeInstance !2
  // Inst[i+1] ==> [no metadata]
  // Inst[i+2] ==> !pipe.pipeName !0, !pipe.pipeLine !1, !pipe.pipeInstance !2
  //
  MDNode *NameNode = nullptr;
  MDNode *LineNode = nullptr;
  for( unsigned i=0; i<(PipeName.size()-3); i++ ){
    if( (PipeName[i] != ".") && (PipeName[i+2] != ".") ){
      if( PipeName[i+1] == "." ){
        // inner pipe has no metadata
        if( (PipeName[i] == PipeName[i+2]) &&
            (PipeLine[i] == PipeLine[i+2]) ){

          // outer values are the same, set the inner value
          NameNode = MDNode::get(TheModule->getContext(),
                                 MDString::get(TheModule->getContext(),
                                               PipeName[i]));
          LineNode = MDNode::get(TheModule->getContext(),
                                 MDString::get(TheModule->getContext(),
                                               PipeLine[i]));
          SetMetadata(BB,i+1,NameNode,LineNode);
        }
      }
    }
  }// end for
}

void SCMetadataAdjust::AdjustMD(){
  // walk all the functions
  for( auto &Func : TheModule->getFunctionList() ){
    // walk all the basic blocks
    for( auto &BB : Func.getBasicBlockList() ){
      // examine the current basic block for metadata
      // continuity
      ExamineBBMetadata(BB);
    }
  }
}

bool SCMetadataAdjust::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // execute the body of the pass
  AdjustMD();

  return true;
}

// EOF
