//
// _CoreGenCache_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenVLIWStage.h"

CoreGenVLIWStage::CoreGenVLIWStage(std::string N,
                                   CoreGenInstFormat *F,
                                   CoreGenErrno *E)
  : CoreGenNode(CGVLIW,N,E), Id(0), Format(F) {
  if( Format )
    InsertChild(static_cast<CoreGenNode *>(Format));

  if( !ParseVLIWStage(N) )
    E->SetError(CGERR_ERROR, "Invalid VLIWStage identifier");
}

CoreGenVLIWStage::~CoreGenVLIWStage(){
}

bool CoreGenVLIWStage::ParseVLIWStage(std::string Name){
  Id = 0;
  std::string delimiter = "_";
  size_t pos = Name.find_last_of(delimiter);

  if( pos == std::string::npos ){
    return false;
  }

  pos++;

  // parse the name to find the form:
  // NAME_xx
  // where "NAME" is the stage name
  // and "xx" is the stage number 0->n
  std::string token = Name.substr(pos,Name.size());

  if( token.length() == 0 ){
    return false;
  }

  for(std::string::iterator it=token.begin(); it!=token.end(); ++it){
    if( !std::isdigit(*it) )
      return false;
  }

  Id = std::stoi(token);

  return true;
}

bool CoreGenVLIWStage::SetFormat(CoreGenInstFormat *F){
  Format = F;
  return true;
}

bool CoreGenVLIWStage::SetID(unsigned I){
  Id = I;
  return true;
}

bool CoreGenVLIWStage::InsertInput(std::string I){
  Inputs.push_back(I);
  return true;
}

bool CoreGenVLIWStage::InsertOutput(std::string O){
  Outputs.push_back(O);
  return true;
}

bool CoreGenVLIWStage::DeleteInputByName(std::string N){
  for( unsigned i=0; i<Inputs.size(); i++ ){
    if( Inputs[i] == N ){
      // delete this
      Inputs.erase(Inputs.begin()+i);
      return true;
    }
  }
  return false;
}

bool CoreGenVLIWStage::DeleteOutputByName(std::string O){
  for( unsigned i=0; i<Outputs.size(); i++ ){
    if( Outputs[i] == O ){
      // delete this
      Outputs.erase(Outputs.begin()+i);
      return true;
    }
  }
  return false;
}

bool CoreGenVLIWStage::SetNullFormat(){
  Format = nullptr;
  return true;
}

// EOF
