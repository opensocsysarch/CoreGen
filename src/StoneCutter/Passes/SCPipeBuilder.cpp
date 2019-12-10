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
  : SCPass("PipeBuilder",TM,O,M) {
}

SCPipeBuilder::~SCPipeBuilder(){
}

bool SCPipeBuilder::BuildArithPipe(){

  // walk all the functions
  for(auto &Func : TheModule->getFunctionList() ){
    // walk all the basic blocks
    for(auto &BB : Func.getBasicBlockList() ){
      // walk all the instructions
      for(auto &Inst : BB.getInstList() ){
        // evaluate each instruction
        if( Inst.isBinaryOp() || Inst.isBitwiseLogicOp() ){
          ArithOps.push_back(Inst.getOpcode());
        }else if( Inst.isShift() ){
          ShiftOps.push_back(Inst.getOpcode());
        }
      }
    }
  }

  return true;
}

bool SCPipeBuilder::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  if( !BuildArithPipe() ){
    return false;
  }

  return true;
}

// EOF
