//
// _SCPipeInfo_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCPipeInfo.h"

SCPipeInfo::SCPipeInfo(Module *TM, SCOpts *O, SCMsg *M)
  : TheModule(TM), Opts(O), Msgs(M), PCPresent(false) {
    PCReg.first = "PC";
    PCReg.second = "SIGTEMP";
}

SCPipeInfo::~SCPipeInfo(){
}

std::string SCPipeInfo::GetRegFieldInstFormat( unsigned Idx ){
  if( Idx > (RegFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Reg field index out of range" );
    return "";
  }

  return RegFields[Idx].first;
}

std::string SCPipeInfo::GetRegFieldName( unsigned Idx ){
  if( Idx > (RegFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Reg field index out of range" );
    return "";
  }

  return RegFields[Idx].second;
}

std::string SCPipeInfo::GetUniqueFieldName( unsigned Idx ){
  if( Idx > (UniqueRegFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Unique reg field index out of range" );
    return "";
  }
  return UniqueRegFields[Idx];
}

std::string SCPipeInfo::GetImmFieldInstFormat( unsigned Idx ){
  if( Idx > (ImmFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Imm field index out of range" );
    return "";
  }
  return ImmFields[Idx].first;
}

std::string SCPipeInfo::GetImmFieldName( unsigned Idx ){
  if( Idx > (ImmFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Imm field index out of range" );
    return "";
  }
  return ImmFields[Idx].second;
}

std::string SCPipeInfo::GetUniqueImmFieldName( unsigned Idx ){
  if( Idx > (UniqueImmFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Unique imm field index out of range" );
    return "";
  }
  return UniqueImmFields[Idx];
}

std::string SCPipeInfo::GetPCName(){
  return PCReg.second;
}

std::string SCPipeInfo::GetPCRegClass(){
  return PCReg.first;
}

// EOF
