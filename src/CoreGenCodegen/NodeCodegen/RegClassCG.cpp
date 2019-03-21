//
// _RegClassCG_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/NodeCodegen/RegClassCG.h"

RegClassCG::RegClassCG( CoreGenNode *N, CoreGenProj *P,
                        std::string Path, CoreGenErrno *E )
  : CoreGenNodeCodegen(N,P,Path,E) {
}

RegClassCG::~RegClassCG(){
}

bool RegClassCG::Execute(){
  return true;
}

// EOF
