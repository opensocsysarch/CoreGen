//
// _SCMetadataAdjust_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCMetadataAdjust.h"

SCMetadataAdjust::SCMetadataAdjust(Module *TM,
                     SCOpts *O,
                     SCMsg *M)
  : SCPass("MetadataAdjust","",TM,O,M), Distance(2) {
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
  if( BB.size() < Distance )
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
  // We need to examine the i'th to the i'th+Distance
  // instructions and the associated metadata.
  // If the inner most instruction does not
  // have metadata and the outer (halo) instructions
  // contain identical metadata, then set the inner
  // instruction metadata to match the adjacent pair.
  // In this case, we retrieve the pipeName and pipeLine
  // for each instruction.
  //
  // for k=i+1, k<(i+Distance)
  //
  // Inst[i]          ==> !pipe.pipeName !0, !pipe.pipeLine !1, !pipe.pipeInstance !2
  // Inst[k]          ==> [no metadata]
  // Inst[i+Distance] ==> !pipe.pipeName !0, !pipe.pipeLine !1, !pipe.pipeInstance !2
  //
  MDNode *NameNode = nullptr;
  MDNode *LineNode = nullptr;
  for( unsigned i=0; i<(PipeName.size()-Distance); i++ ){
    if( (PipeName[i] != ".") && (PipeName[i+Distance] != ".") ){
      for( unsigned k=(i+1); k<(i+Distance); k++ ){
        if( PipeName[k] == "." ){
          // inner statement has no metadata
          if( (PipeName[i] == PipeName[i+Distance]) &&
              (PipeLine[i] == PipeLine[i+Distance]) ){
            // outer values are the same, set the inner value
            NameNode = MDNode::get(TheModule->getContext(),
                                  MDString::get(TheModule->getContext(),
                                               PipeName[i]));
            LineNode = MDNode::get(TheModule->getContext(),
                                  MDString::get(TheModule->getContext(),
                                               PipeLine[i]));
            SetMetadata(BB,k,NameNode,LineNode);
          }
        }// inner if stmt
      }// end k loop
    } // end outer if stmt
  }// end for
}

void SCMetadataAdjust::AdjustMD(){
  if( Opts->IsVerbose() ){
    this->PrintRawMsg("Using metadata distance of " + std::to_string(Distance));
  }

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

void SCMetadataAdjust::Split(const std::string& s, char delim,
                      std::vector<std::string>& v) {
  auto i = 0;
  auto pos = s.find(delim);
  if( pos == std::string::npos ){
    v.push_back(s.substr(i,s.length()));
  }
  while( pos != std::string::npos ){
    v.push_back(s.substr(i,pos-i));
    i = ++pos;
    pos = s.find(delim,pos);
    if( pos == std::string::npos ){
      v.push_back(s.substr(i,s.length()));
    }
  }
}

bool SCMetadataAdjust::ReadPassOptions(){
  std::string Options = GetSCPassOptions();
  std::vector<std::string> V;
  if( Options.length() > 0 ){
    Split(Options,'=',V);

    if( V.size() != 2 ){
      return true;
    }else if( V[0] == "Distance" ){
      Distance = (unsigned)(std::stoul(V[1],nullptr,0));
      return true;
    }else{
    }
  }
  return true;
}

bool SCMetadataAdjust::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // read the pass options
  if( !ReadPassOptions() ){
    this->PrintMsg( L_ERROR, "Failed to read stonecutter pass options");
    return false;
  }

  // execute the body of the pass
  AdjustMD();

  return true;
}

// EOF
