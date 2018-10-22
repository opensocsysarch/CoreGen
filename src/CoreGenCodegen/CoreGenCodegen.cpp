//
// _CoreGenCodegen_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenCodegen.h"

CoreGenCodegen::CoreGenCodegen(std::string BD, CoreGenErrno *E)
  : BaseDir(BD), Errno(E) {
}

CoreGenCodegen::~CoreGenCodegen(){
}

bool CoreGenCodegen::Execute(){
  return true;
}

// EOF
