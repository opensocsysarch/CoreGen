//
// _CoreGenErrno_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

CoreGenErrno::CoreGenErrno() : ErrCode(CGERR_NONE), ErrStr("") {}

CoreGenErrno::~CoreGenErrno() {}

bool CoreGenErrno::SetError( CGERR Code, std::string Str ){
  ErrCode = Code;
  ErrStr = "\033[1;31m" + Str + "\033[0m";
  return true;
}

bool CoreGenErrno::GetError( CGERR *Code, std::string *Str ){
  if( Code == nullptr ){
    return false;
  }else if( Str == nullptr ){
    return false;
  }
  *Code = ErrCode;
  *Str = ErrStr;
  return true;
}

bool CoreGenErrno::IsFatal(){
  if( ErrCode == CGERR_FATAL ){
    return true;
  }
  return false;
}

std::string CoreGenErrno::GetErrStr(){
  return ErrStr;
}


// EOF
