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


