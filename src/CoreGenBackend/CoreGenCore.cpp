//
// _CoreGenCore_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenCore.h"

CoreGenCore::CoreGenCore( std::string N, CoreGenISA *I, CoreGenErrno *E)
  : CoreGenNode(CGCore,N,E), Cache(NULL), ISA(I), ThreadUnits(1),
    Sched(SMTUnk){
  if( ISA )
    InsertChild(static_cast<CoreGenNode *>(ISA));
}

CoreGenCore::~CoreGenCore(){
}

bool CoreGenCore::SetSched( CGSched S ){
  Sched = S;
  return true;
}

bool CoreGenCore::SetISA( CoreGenISA *I ){
  if( ISA ){
    DeleteChild(static_cast<CoreGenNode *>(ISA));
  }
  ISA = I;
  return InsertChild(static_cast<CoreGenNode *>(ISA));
}

bool CoreGenCore::InsertCache( CoreGenCache *C ){
  if( Cache ){
    DeleteChild(static_cast<CoreGenNode *>(Cache));
  }
  Cache = C;
  return InsertChild(static_cast<CoreGenNode *>(Cache));
}

bool CoreGenCore::InsertRegClass( CoreGenRegClass *RegClass ){
  // check for a collision
  for( unsigned i=0; i<Regs.size(); i++ ){
    if( Regs[i] == RegClass ){
      // already present
      return true;
    }
  }

  // insert it
  Regs.push_back(RegClass);
  return InsertChild(static_cast<CoreGenNode *>(RegClass));
}

bool CoreGenCore::InsertExt( CoreGenNode *E ){
  // check for a collision
  for( unsigned i=0; i<Exts.size(); i++ ){
    if( Exts[i] == E ){
      // already present
      return true;
    }
  }

  // insert it
  Exts.push_back(E);
  return InsertChild(static_cast<CoreGenNode *>(E));
}

CoreGenRegClass *CoreGenCore::GetRegClass( unsigned C ){
  if( C > (Regs.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Register class " + std::to_string(C)
              + " out of bounds");
    return NULL;
  }

  return Regs[C];
}

bool CoreGenCore::DeleteRegClass( unsigned C ){
  if( C > (Regs.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Register class " + std::to_string(C)
              + " out of bounds");
    return false;
  }

  Regs.erase(Regs.begin()+C);
  return true;
}

bool CoreGenCore::DeleteExt( unsigned C ){
  if( C > (Exts.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Register class " + std::to_string(C)
              + " out of bounds");
    return false;
  }

  Exts.erase(Exts.begin()+C);
  return true;
}

CoreGenNode *CoreGenCore::GetExt( unsigned E ){
  if( E > (Exts.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Extension" + std::to_string(E)
              + " out of bounds");
    return NULL;
  }

  return Exts[E];
}

bool CoreGenCore::SetNumThreadUnits( unsigned TU ){
  if( TU == 0 ){
    Errno->SetError( CGERR_ERROR, "Cannot set thread units to zero" );
    return false;
  }
  ThreadUnits = TU;
  return true;
}

std::string CoreGenCore::CGSchedToStr( CGSched S ){
  switch( S ){
  case SMTUnk:
    return "Unknown";
    break;
  case SMTPCycle:
    return "PCycle";
    break;
  case SMTPressure:
    return "Pressure";
    break;
  case SMTProg:
    return "Programmatic";
    break;
  default:
    return "";
    break;
  }
  return "";
}

CGSched CoreGenCore::StrToCGSched( std::string Str ){
  if( Str == "Unknown" ){
    return SMTUnk;
  }else if( Str == "PCycle" ){
    return SMTPCycle;
  }else if( Str == "Pressure" ){
    return SMTPressure;
  }else if( Str == "Programmatic" ){
    return SMTProg;
  }else{
    return SMTUnk;
  }
  return SMTUnk;
}

// EOF
