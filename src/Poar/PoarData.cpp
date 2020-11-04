//
// _PoarData_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/PoarData.h"

PoarData::PoarData(PoarOpts *P)
  : POpts(P), PConfig(nullptr), CG(nullptr){
}

PoarData::~PoarData(){
  delete PConfig;
  delete CG;
}

bool PoarData::Init(){
  // ensure that the options object is valid
  if( !POpts ){
    ErrStr = "Options object not initialized";
    return false;
  }

  // load a configuration template (or the default)
  PConfig = new PoarConfig(POpts->GetConfig());
  if( !PConfig ){
    ErrStr = "Could not load power/area configuration data";
    return false;
  }

  // load a coregen design object
  CG = new CoreGenBackend();
  if( !CG ){
    ErrStr = "Could not load CoreGen design data";
    return false;
  }

  if( !CG->ReadIR(POpts->GetDesignFile()) ){
    ErrStr = "Could not read design input file : " + CG->GetErrStr();
    return false;
  }

  return true;
}

// EOF
