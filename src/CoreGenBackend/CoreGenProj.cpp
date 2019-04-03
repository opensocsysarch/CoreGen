//
// _CoreGenProj_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenProj.h"

CoreGenProj::CoreGenProj( CoreGenErrno *E) : Errno(E) {}

CoreGenProj::CoreGenProj( CoreGenErrno *E,
               std::string N,
               std::string R ):
    Errno(E), ProjName(N), ProjRoot(R), Type(CGNULLProj),
    ChiselMajor(3), ChiselMinor(0) {}

CoreGenProj::CoreGenProj( CoreGenErrno *E,
               std::string N,
               std::string R,
               CGProjType T ) :
    Errno(E), ProjName(N), ProjRoot(R), Type(T),
    ChiselMajor(3), ChiselMinor(0) {}

CoreGenProj::~CoreGenProj(){}

bool CoreGenProj::SetChiselVersion( unsigned Major, unsigned Minor ){
    ChiselMajor = Major; ChiselMinor = Minor; return true;
}

bool CoreGenProj::SetProjName(std::string N){
  if( N.length() == 0 ){
    Errno->SetError(CGERR_ERROR, "Project name is null");
    return false;
  }
  ProjName = N;
  return true;
}

bool CoreGenProj::SetProjRoot(std::string R){
  if( R.length() == 0 ){
    Errno->SetError(CGERR_ERROR, "Project root is null" );
    return false;
  }
  ProjRoot = R;
  return true;
}

bool CoreGenProj::GetChiselVersion( unsigned *Major, unsigned *Minor ){
    *Major = ChiselMajor; *Minor = ChiselMinor; return true;
}

std::string CoreGenProj::GetProjName() { return ProjName; }

std::string CoreGenProj::GetProjRoot() { return ProjRoot; }

bool CoreGenProj::SetProjType( CGProjType T) { Type = T; return true; }

CGProjType CoreGenProj::GetProjType() { return Type; }

// EOF
