//
// _CoreGenLLVMCodegen_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenLLVMCodegen.h"

CoreGenLLVMCodegen::CoreGenLLVMCodegen( CoreGenNode *T,
                                        CoreGenProj *P,
                                        std::string R,
                                        CoreGenErrno *E )
  : Top(T), Proj(P), LLVMRoot(R), Errno(E) {
}

CoreGenLLVMCodegen::~CoreGenLLVMCodegen(){
}

bool CoreGenLLVMCodegen::Execute(){
  return true;
}

// EOF
