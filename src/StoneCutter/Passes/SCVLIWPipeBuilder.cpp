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

bool SCVLIWPipeBuilder::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  return true;
}

// EOF

