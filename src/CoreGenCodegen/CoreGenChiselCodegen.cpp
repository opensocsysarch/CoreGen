//
// _CoreGenChiselCodegen_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenChiselCodegen.h"

CoreGenChiselCodegen::CoreGenChiselCodegen( CoreGenNode *T,
                                            CoreGenProj *P,
                                            std::string R,
                                            CoreGenErrno *E )
  : Top(T), Proj(P), ChiselRoot(R), Errno(E) {
}

CoreGenChiselCodegen::~CoreGenChiselCodegen(){
}

bool CoreGenChiselCodegen::Execute(){
  // walk all the nodes and codegen each node individually
  bool rtn = true;

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    // codegen the i'th node
    switch( Top->GetChild(i)->GetType() ){
    case CGSoc:
      break;
    case CGCore:
      break;
    case CGInstF:
      break;
    case CGInst:
      break;
    case CGPInst:
      break;
    case CGRegC:
      break;
    case CGReg:
      break;
    case CGISA:
      break;
    case CGCache:
      break;
    case CGEnc:
      break;
    case CGExt:
      break;
    case CGComm:
      break;
    case CGSpad:
      break;
    case CGMCtrl:
      break;
    case CGVTP:
      break;
    case CGPlugin:
      break;
    default:
      break;
    }
  }
  return rtn;
}

// EOF
