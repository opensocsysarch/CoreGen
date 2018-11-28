//
// _CoreGenReg_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenReg.h"

CoreGenReg::CoreGenReg(std::string N, int I, int W,
                       CoreGenErrno *E)
  : CoreGenNode(CGReg,N,E), isFixedValue(false), isSIMD(false), isIdxSet(true),
    isShared(false),
    index(I), width(W), SIMDwidth(1), attrs(1<<CoreGenReg::CGRegRW){
}

std::string CoreGenReg::GetPseudoName(){
  std::string N = pName;
  std::transform( N.begin(), N.end(), N.begin(), ::tolower );
  return N;
}

bool CoreGenReg::SetSIMD( int width ){
  isSIMD = true;
  SIMDwidth = width;
  return true;
}

bool CoreGenReg::SetIndex( int Index ){
  index = Index;
  isIdxSet = true;
  return true;
}

bool CoreGenReg::SetPseudoName( std::string PseudoName ){
  pName = PseudoName;
  return true;
}

bool CoreGenReg::IsRWAttr(){
  if( (attrs & CoreGenReg::CGRegRW) > 0 ){
    return true;
  }
  return false;
}

bool CoreGenReg::IsROAttr(){
  if( (attrs & CoreGenReg::CGRegRO) > 0 ){
    return true;
  }
  return false;
}

bool CoreGenReg::IsCSRAttr(){
  if( (attrs & CoreGenReg::CGRegCSR) > 0 ){
    return true;
  }
  return false;
}

bool CoreGenReg::IsAMSAttr(){
  if( (attrs & CoreGenReg::CGRegAMS) > 0 ){
    return true;
  }
  return false;
}

bool CoreGenReg::IsTUSAttr(){
  if( (attrs & CoreGenReg::CGRegTUS) > 0 ){
    return true;
  }
  return false;
}

bool CoreGenReg::SetShared( bool S ){
  if( S ){
    if( (attrs & CoreGenReg::CGRegTUS) > 0 ){
      // registers shared across cores cannot be TUS
      // if all cores can see the reg, then all threads
      // can see the reg
      attrs &= ~(1 << CoreGenReg::CGRegTUS);
    }
  }
  isShared = S;
  return true;
}

bool CoreGenReg::UnsetAttrs( uint32_t Attr ){
  if( (Attr & CoreGenReg::CGRegRO) > 0 ){
    attrs &= ~(1 << CoreGenReg::CGRegRO);
  }
  if( (Attr & CoreGenReg::CGRegRW) > 0 ){
    attrs &= ~(1 << CoreGenReg::CGRegRW);
  }
  if( (Attr & CoreGenReg::CGRegAMS) > 0 ){
    attrs &= ~(1 << CoreGenReg::CGRegAMS);
  }
  if( (Attr & CoreGenReg::CGRegCSR) > 0 ){
    attrs &= ~(1 << CoreGenReg::CGRegCSR);
  }
  if( (Attr & CoreGenReg::CGRegTUS) > 0 ){
    attrs &= ~(1 << CoreGenReg::CGRegTUS);
  }
  return true;
}

bool CoreGenReg::SetAttrs( uint32_t Attr ){
  if( ((Attr & CoreGenReg::CGRegRO)>0) &&
      ((Attr & CoreGenReg::CGRegRW)>0) ){
    // cannot set RW & RO
    Errno->SetError( CGERR_ERROR,
       "Cannot Set RO and RW attributes : Register=" + this->GetName());
    return false;
  }else if( ((Attr & CoreGenReg::CGRegCSR)>0) &&
      ((Attr & CoreGenReg::CGRegAMS)>0) ){
    // cannot set RW & RO
    Errno->SetError( CGERR_ERROR,
       "Cannot Set CSR and AMS attributes : Register=" + this->GetName());
    return false;
  }

  if( (Attr & CoreGenReg::CGRegRO) > 0 ){
    // unset RW
    attrs &= ~(1 << CoreGenReg::CGRegRW);
  }else if( (Attr & CoreGenReg::CGRegRW) > 0 ){
    // unset RO
    attrs &= ~(1 << CoreGenReg::CGRegRO);
  }else if( (Attr & CoreGenReg::CGRegCSR) > 0 ){
    // unset AMS
    attrs &= ~(1 << CoreGenReg::CGRegAMS);
  }else if( (Attr & CoreGenReg::CGRegAMS) > 0 ){
    // unset CSR
    attrs &= ~(1 << CoreGenReg::CGRegCSR);
  }else if( (Attr & CoreGenReg::CGRegTUS) > 0 ){
    // TUS registers cannot be shared across cores
    isShared = false;
  }

  attrs |= Attr;
  return true;
}

bool CoreGenReg::SetFixedVals( std::vector<uint64_t> FixedVals ){
  if( FixedVals.size() == 0 ){
    return false;
  }
  isFixedValue = true;
  for( unsigned i=0; i<FixedVals.size(); i++ ){
    fixedVals.push_back(FixedVals[i]);
  }
  return true;
}

bool CoreGenReg::SetFixedVals( uint64_t FixedVal, unsigned Idx ){
  if( Idx > (fixedVals.size()-1) ){
    return false;
  }
  fixedVals[Idx] = FixedVal;
  return true;
}

bool CoreGenReg::GetFixedVals( std::vector<uint64_t> &FixedVals ){
  if( isFixedValue ){
    for( unsigned i=0; i<fixedVals.size(); i++ ){
      FixedVals.push_back(fixedVals[i]);
    }
    return true;
  }
  return false;
}

bool CoreGenReg::GetSubReg( unsigned Idx,
                            std::string &Name,
                            unsigned &Start,
                            unsigned &End ){
  if( Idx > (SubRegs.size()-1) ){
    Errno->SetError(CGERR_ERROR, "Subregister index out of bound: "
                    + std::to_string(Idx ) );
    return false;
  }

  // retrieve all the data
  Name  = std::get<0>(SubRegs[Idx]);
  Start = std::get<1>(SubRegs[Idx]);
  End   = std::get<2>(SubRegs[Idx]);

  return true;
}

bool CoreGenReg::InsertSubReg( std::string Name, unsigned Start, unsigned End ){
  if( Name.length() == 0 ){
    Errno->SetError(CGERR_ERROR,
                    "Subregister name is null: " + this->GetName() + ":" + Name);
    return false;
  }else if( End == 0 ){
    Errno->SetError(CGERR_ERROR,
                    "Subregister end bit cannot be 0: "
                    + this->GetName() + ":" + Name );
    return false;
  }else if( End < Start ){
    Errno->SetError(CGERR_ERROR,
                    "Subregister start and/or end bits out of range: "
                    + this->GetName() + ":" + Name );
    return false;
  }else if( (int)(End) >= width ){
    Errno->SetError(CGERR_ERROR,
                    "Subregister end bit beyond register width: "
                    + this->GetName() + ":" + Name );
    return false;
  }
  SubRegs.push_back(std::tuple<std::string,unsigned,unsigned>(Name,Start,End));
  return true;
}

CoreGenReg::~CoreGenReg() {
}

// EOF
