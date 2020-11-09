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

  // retrieve the pipeline stage vector
  // InstVect = SigMap->GetInstVect();

  // //std::cout << sizeof(PipeVect) << std::endl;

  // // allocate the matrix
  // if( !AllocMat() ){
  //   this->PrintMsg( L_ERROR, "Could not allocate matrix" );
  //   return false;
  // }

  // // build the pipeline matrix
  // if( !BuildMat() ){
  //   this->PrintMsg( L_ERROR, "Could not build matrix representation of pipeline" );
  //   FreeMat();
  //   return false;
  // }

  // //PrintAdjMat();
  // // free the matrix
  // if( !FreeMat() ){
  //   this->PrintMsg( L_ERROR, "Could not free adjacency matrix" );
  //   return false;
  // }

  return true;
}