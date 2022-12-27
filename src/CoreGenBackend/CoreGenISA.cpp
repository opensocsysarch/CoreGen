//
// _CoreGenISA_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenISA.h"

CoreGenISA::CoreGenISA(CoreGenErrno *E)
  : CoreGenNode(CGISA,E), isVLIW(false) {
}

CoreGenISA::CoreGenISA(std::string N, CoreGenErrno *E)
  : CoreGenNode(CGISA,N,E), isVLIW(false) {
}

CoreGenISA::~CoreGenISA(){
}

// EOF
