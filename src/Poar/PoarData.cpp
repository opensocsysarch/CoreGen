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
  : POpts(P), PConfig(nullptr), CG(nullptr), SM(nullptr), Top(nullptr){
}

PoarData::~PoarData(){
  delete PConfig;
  if( CG )
    delete CG;
  if( SM )
    delete SM;
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
  if( POpts->GetDesignFile().length() > 0 ){
    CG = new CoreGenBackend();
    if( !CG ){
      ErrStr = "Could not load CoreGen design data";
      return false;
    }

    if( !CG->ReadIR(POpts->GetDesignFile()) ){
      ErrStr = "Could not read design input file : " + CG->GetErrStr();
      return false;
    }

    Top = CG->GetTop();
    if( POpts->GetRoot().length() > 0 ){
      if( !CG->GetNodeByName(POpts->GetRoot()) ){
        ErrStr = "Root node does not exist in design : " + POpts->GetRoot();
        return false;
      }
      Top = CG->GetNodeByName(POpts->GetRoot());
    }
  }

  // load a stonecutter object
  if( POpts->GetSigMapFile().length() > 0 ){
    SM = new CoreGenSigMap();
    if( !SM ){
      ErrStr = "Could not not CoreGenSigMap signal map data";
      return false;
    }

    if( !SM->ReadSigMap(POpts->GetSigMapFile()) ){
      ErrStr = "Could not read signal map input file : " + SM->GetErrStr();
      return false;
    }
  }

  // init all the accumulators
  if( !InitAccum() ){
    ErrStr = "Could not initialize POAR accumulators";
    return false;
  }

  return true;
}

PoarAccum * PoarData::GetAccumByName(std::string N){
  for( unsigned i=0; i<Accums.size(); i++ ){
    if( Accums[i]->GetName() == N )
      return Accums[i];
  }

  return nullptr;
}

bool PoarData::InitAccum(){
  Accums.push_back(static_cast<PoarAccum *>(new PoarRegAccum(Top)));
  return true;
}

bool PoarData::DeriveData(){
  // walk all the entries
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    // retrieve the equivalent accumulator object
    PoarAccum *PA = GetAccumByName(CE.Accum);

    // execute the accumulator
    if( PA ){
      PA->Accum();
    }
  }
  return true;
}

bool PoarData::WriteData(){
  if( !POpts ){
    ErrStr = "Options object not initialized";
    return false;
  }

  PoarIO *PIO = new PoarIO();
  if( !PIO ){
    ErrStr = "Could not init IO object";
    return false;
  }

  // init the specific i/o options
  if( POpts->IsTextOutput() ){
  }else if( POpts->IsYamlOutput() ){
  }else if( POpts->IsLatexOutput() ){
  }else if( POpts->IsXmlOutput() ){
  }

  // write out all the data

  delete PIO;

  return true;
}

// EOF
