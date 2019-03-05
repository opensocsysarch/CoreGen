//
// _CoreGenExt_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenExt.h"

CoreGenExt::CoreGenExt(std::string N, CoreGenErrno *E)
  : CoreGenNode(CGExt,N,E),
  Type(CGExtUnk){
}

CoreGenExt::~CoreGenExt(){
  for( unsigned i=0; i<Cores.size(); i++ ){
    delete Cores[i];
  }
  for( unsigned i=0; i<Caches.size(); i++ ){
    delete Caches[i];
  }
  for( unsigned i=0; i<Exts.size(); i++ ){
    delete Exts[i];
  }
  for( unsigned i=0; i<Insts.size(); i++ ){
    delete Insts[i];
  }
  for( unsigned i=0; i<Formats.size(); i++ ){
    delete Formats[i];
  }
  for( unsigned i=0; i<Regs.size(); i++ ){
    delete Regs[i];
  }
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    delete RegClasses[i];
  }
  for( unsigned i=0; i<ISAs.size(); i++ ){
    delete ISAs[i];
  }
  for( unsigned i=0; i<Comms.size(); i++ ){
    delete Comms[i];
  }
}

bool CoreGenExt::SetType(CGExtType T){
  if( (T!=CGExtTemplate) &&
      (T!=CGExtModule) &&
      (T!=CGExtComm) &&
      (T!=CGExtUnk) ){
    return false;
  }
  Type = T;
  return true;
}

CoreGenCache *CoreGenExt::InsertCache(std::string Name,
                          unsigned Sets,
                          unsigned Ways){
  CoreGenCache *C = new CoreGenCache(Name,Sets,Ways,Errno);
  Caches.push_back(C);
  CoreGenNode *N = static_cast<CoreGenNode *>(C);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return C;
}

CoreGenCore *CoreGenExt::InsertCore( std::string Name,
                           CoreGenISA *ISA ){
  CoreGenCore *C = new CoreGenCore(Name, ISA, Errno);
  Cores.push_back(C);
  CoreGenNode *N = static_cast<CoreGenNode *>(C);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return C;
}

CoreGenComm *CoreGenExt::InsertComm( std::string Name ){
  CoreGenComm *C = new CoreGenComm(Name,Errno);
  Comms.push_back(C);
  CoreGenNode *N = static_cast<CoreGenNode *>(C);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return C;
}

CoreGenInst *CoreGenExt::InsertInst( std::string Name,
                           CoreGenISA *ISA,
                           CoreGenInstFormat *Format ){
  CoreGenInst *I = new CoreGenInst(Name,ISA,Format,Errno);
  Insts.push_back(I);
  CoreGenNode *N = static_cast<CoreGenNode *>(I);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return I;
}

CoreGenPseudoInst *CoreGenExt::InsertPseudoInst( std::string Name,
                                       CoreGenInst *Inst ){
  CoreGenPseudoInst *P = new CoreGenPseudoInst(Name,Inst,Errno);
  PInsts.push_back(P);
  CoreGenNode *N = static_cast<CoreGenNode *>(P);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return P;
}

CoreGenInstFormat *CoreGenExt::InsertInstFormat( std::string Name,
                                       CoreGenISA *ISA ){
  CoreGenInstFormat *IF = new CoreGenInstFormat(Name,ISA,Errno);
  Formats.push_back(IF);
  CoreGenNode *N = static_cast<CoreGenNode *>(IF);
  this->InsertChild(N);
  return IF;
}

CoreGenReg *CoreGenExt::InsertReg( std::string Name,
                                   int RegIndex,
                                   int RegWidth ){
  CoreGenReg *R = new CoreGenReg(Name,RegIndex,RegWidth,Errno);
  Regs.push_back(R);
  CoreGenNode *N = static_cast<CoreGenNode *>(R);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return R;
}

CoreGenRegClass *CoreGenExt::InsertRegClass( std::string Name ){
  CoreGenRegClass *RC = new CoreGenRegClass(Name,Errno);
  RegClasses.push_back(RC);
  CoreGenNode *N = static_cast<CoreGenNode *>(RC);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return RC;
}

CoreGenISA *CoreGenExt::InsertISA( std::string Name ){
  CoreGenISA *I = new CoreGenISA(Name,Errno);
  ISAs.push_back(I);
  CoreGenNode *N = static_cast<CoreGenNode *>(I);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return I;
}

bool CoreGenExt::InsertRegClass( CoreGenRegClass *R ){
  if( !R ){
    return false;
  }
  RegClasses.push_back(R);
  CoreGenNode *N = static_cast<CoreGenNode *>(R);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertReg( CoreGenReg *R ){
  if( !R ){
    return false;
  }
  Regs.push_back(R);
  CoreGenNode *N = static_cast<CoreGenNode *>(R);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertISA( CoreGenISA *I ){
  if( !I ){
    return false;
  }
  ISAs.push_back(I);
  CoreGenNode *N = static_cast<CoreGenNode *>(I);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertInstFormat( CoreGenInstFormat *I ){
  if( !I ){
    return false;
  }
  Formats.push_back(I);
  CoreGenNode *N = static_cast<CoreGenNode *>(I);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertInst( CoreGenInst *I ){
  if( !I ){
    return false;
  }
  Insts.push_back(I);
  CoreGenNode *N = static_cast<CoreGenNode *>(I);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertPseudoInst( CoreGenPseudoInst *I ){
  if( !I ){
    return false;
  }
  PInsts.push_back(I);
  CoreGenNode *N = static_cast<CoreGenNode *>(I);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertCache( CoreGenCache *C ){
  if( !C ){
    return false;
  }
  Caches.push_back(C);
  CoreGenNode *N = static_cast<CoreGenNode *>(C);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertCore( CoreGenCore *C ){
  if( !C ){
    return false;
  }
  Cores.push_back(C);
  CoreGenNode *N = static_cast<CoreGenNode *>(C);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertComm( CoreGenComm *C ){
  if( !C ){
    return false;
  }
  Comms.push_back(C);
  CoreGenNode *N = static_cast<CoreGenNode *>(C);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertExt( CoreGenExt *E ){
  if( !E ){
    return false;
  }
  Exts.push_back(E);
  CoreGenNode *N = static_cast<CoreGenNode *>(E);
  return this->InsertChild(N);
}

// EOF
