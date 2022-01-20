//
// _CoreGenDataPath_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenDataPath.h"

CoreGenDataPath::CoreGenDataPath(CoreGenErrno *E)
  : CoreGenNode(CGDPath,E), Style("unknown"){
}

CoreGenDataPath::CoreGenDataPath(std::string Name, std::string S, CoreGenErrno *E)
  : CoreGenNode(CGDPath,Name,E){
    Style = S;
}

CoreGenDataPath::~CoreGenDataPath(){
}

// EOF
