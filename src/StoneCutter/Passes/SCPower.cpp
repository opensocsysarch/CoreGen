//
// _SCPower_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCPower.h"

SCPower::SCPower(Module *TM,
                             SCOpts *O,
                             SCMsg *M)
  : SCPass("Power","",TM,O,M) {
}

SCPower::~SCPower(){
}

void SCPower::GetActiveCycles(){
  std::cout << GetNumPipelines() << std::endl;
}

bool SCPower::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  if( !SigMap ){
    this->PrintMsg(L_ERROR, "SigMap is null");
    return false;
  }

  PipeVect = SigMap->GetPipeVect();
  InstVect = SigMap->GetInstVect();

  for( unsigned i=0; i<InstVect.size(); i++ ){
    // Get vector of signals for every instruction
    std::vector<SCSig *> SigVect = SigMap->GetSigVect(InstVect[i]);
    unsigned DupArithCount = 0;
    // For each SigVect, see if any have duplicate ALU_* symbols (ie. EXE stage for more than 1 cycle)
    for( unsigned j=1; j<SigVect.size(); j++){
      SigType PrevSigType = SigVect[j-1]->GetType();
      // TODO: Could probably turn this into a signal line Ternary
      if( SigVect[j]->isALUSig() && SigVect[j]->GetType() == PrevSigType ){
        DupArithCount++;
      }
      else{ 
        continue;
      }
    }
    std::cout << InstVect[i] << ": " << DupArithCount << std::endl;
  }
  return true;
}


/*** Crap Code ***/

 // ! Accidentally recreated getting cycle count w/out realizing only prob was w/ VLSI & SuperScalar *//
  // std::cout <<SigMap->GetSigVect(SigMap->GetSignal(0)->GetInst()).size() << std::endl;
  // PipeVect = SigMap->GetPipeVect();
  // InstVect = SigMap->GetInstVect();

  // unsigned cyclesForInst = 0; 
  // // Try to get accurate cycle count for instructions (For RISC, should be 5)
  // for (unsigned i=0; i<SigMap->GetNumSignals(); i++){
  //   //SCSig *Next = SigMap->GetSignal(i+1);
  //   unsigned cyclesForInst;
  //   if (SigMap->GetSignal(i)->GetInst() == SigMap->GetSignal(i+1)->GetInst()){
  //     cyclesForInst++;
  //     continue;
  //   }
  //   else{
  //     // std::cout << SigMap->GetSignal(i)->GetInst() << ": " << cyclesForInst << std::endl;
  //     cyclesForInst = 0;
  //     continue;
  //   }

  // }
