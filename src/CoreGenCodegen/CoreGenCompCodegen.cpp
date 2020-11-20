//
// _CoreGenCompCodegen_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenCompCodegen.h"

CoreGenCompCodegen::CoreGenCompCodegen( CoreGenNode *T,
                                        CoreGenProj *P,
                                        CoreGenArchEntry *EN,
                                        std::string R,
                                        CoreGenErrno *E )
  : Top(T), Proj(P), Entry(EN), LLVMRoot(R), Errno(E) {
}

CoreGenCompCodegen::~CoreGenCompCodegen(){
}

bool CoreGenCompCodegen::Execute(){

  CoreGenLLVMCodegen *CG;

  // Determine the version that we're attempting to build the compiler
  // for and execute it
  if( Entry->GetVersion() == "8.0.1" ){
    CG = static_cast<CoreGenLLVMCodegen *>(new LLVM801CG(Top,Proj,Entry,LLVMRoot,Errno));
    if( CG == nullptr ){
      Errno->SetError(CGERR_ERROR, "Failed to create LLVM codegen object for version 8.0.1");
      return false;
    }
  }else if( Entry->GetVersion() == "9.0.1" ){
    CG = static_cast<CoreGenLLVMCodegen *>(new LLVM901CG(Top,Proj,Entry,LLVMRoot,Errno));
    if( CG == nullptr ){
      Errno->SetError(CGERR_ERROR, "Failed to create LLVM codegen object for version 9.0.1");
      return false;
    }
  }else{
    Errno->SetError(CGERR_ERROR, "No codegen exists for version LLVM " + Entry->GetVersion() );
    return false;
  }

  bool rtn = true;
  if( !CG->Execute() )
    rtn = false;

  if( CG ){
    delete CG;
  }

  return rtn;
}

// EOF
