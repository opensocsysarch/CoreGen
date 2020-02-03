//
// _CoreGenDataPath_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenDataPath.h"

CoreGenDataPath::CoreGenDataPath(CoreGenErrno *E)
  : CoreGenNode(CGDPath,E) {
}

CoreGenDataPath::CoreGenDataPath(std::string N, CoreGenErrno *E)
  : CoreGenNode(CGDPath,N,E){
}

CoreGenDataPath::~CoreGenDataPath(){
}

// EOF
