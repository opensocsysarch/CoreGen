//
// _CoreGenExt_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
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
    this->DeleteChild(static_cast<CoreGenNode *>(Cores[i]));
    delete Cores[i];
  }
  for( unsigned i=0; i<Caches.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(Caches[i]));
    delete Caches[i];
  }
  for( unsigned i=0; i<Exts.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(Exts[i]));
    delete Exts[i];
  }
  for( unsigned i=0; i<Insts.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(Insts[i]));
    delete Insts[i];
  }
  for( unsigned i=0; i<Formats.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(Formats[i]));
    delete Formats[i];
  }
  for( unsigned i=0; i<Regs.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(Regs[i]));
    delete Regs[i];
  }
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(RegClasses[i]));
    delete RegClasses[i];
  }
  for( unsigned i=0; i<ISAs.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(ISAs[i]));
    delete ISAs[i];
  }
  for( unsigned i=0; i<Comms.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(Comms[i]));
    delete Comms[i];
  }
  for( unsigned i=0; i<Spads.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(Spads[i]));
    delete Spads[i];
  }
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(MCtrls[i]));
    delete MCtrls[i];
  }
  for( unsigned i=0; i<DataPaths.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(DataPaths[i]));
    delete DataPaths[i];
  }
  for( unsigned i=0; i<VTPs.size(); i++ ){
    this->DeleteChild(static_cast<CoreGenNode *>(VTPs[i]));
    delete VTPs[i];
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

bool CoreGenExt::UpdateChildNodes(){
  this->PurgeChildren();
  for( unsigned i=0; i<Cores.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Cores[i]));
  }
  for( unsigned i=0; i<Caches.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Caches[i]));
  }
  for( unsigned i=0; i<Exts.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Exts[i]));
  }
  for( unsigned i=0; i<Insts.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Insts[i]));
  }
  for( unsigned i=0; i<Formats.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Formats[i]));
  }
  for( unsigned i=0; i<Regs.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Regs[i]));
  }
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(RegClasses[i]));
  }
  for( unsigned i=0; i<ISAs.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(ISAs[i]));
  }
  for( unsigned i=0; i<Comms.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Comms[i]));
  }
  for( unsigned i=0; i<Cores.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Cores[i]));
  }
  for( unsigned i=0; i<Spads.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(Spads[i]));
  }
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(MCtrls[i]));
  }
  for( unsigned i=0; i<DataPaths.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(DataPaths[i]));
  }
  for( unsigned i=0; i<VTPs.size(); i++ ){
    this->InsertChild(static_cast<CoreGenNode *>(VTPs[i]));
  }
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

CoreGenDataPath *CoreGenExt::InsertDataPath( std::string Name,
                                             std::string Style ){
  CoreGenDataPath *D = new CoreGenDataPath(Name, Style, Errno);
  DataPaths.push_back(D);
  CoreGenNode *N = static_cast<CoreGenNode *>(D);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return D;
}

CoreGenVTP *CoreGenExt::InsertVTP(std::string Name){
  CoreGenVTP *V = new CoreGenVTP(Name,Errno);
  VTPs.push_back(V);
  CoreGenNode *N = static_cast<CoreGenNode *>(V);
  if( !this->InsertChild(N) ){
    return nullptr;
  }
  return V;
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

bool CoreGenExt::InsertDataPath( CoreGenDataPath *D ){
  if( !D ){
    return false;
  }
  DataPaths.push_back(D);
  CoreGenNode *N = static_cast<CoreGenNode *>(D);
  return this->InsertChild(N);
}

bool CoreGenExt::InsertVTP( CoreGenVTP *V ){
  if( !V ){
    return false;
  }
  VTPs.push_back(V);
  CoreGenNode *N = static_cast<CoreGenNode *>(V);
  return this->InsertChild(N);
}

// EOF
