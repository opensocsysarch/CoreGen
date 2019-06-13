//
// _CoreGenPluginImpl_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenPlugin/CoreGenPluginImpl.h"

CoreGenPluginImpl::CoreGenPluginImpl(std::string N,
                                     bool HDL,
                                     bool LLVM,
                                     unsigned MaV,
                                     unsigned MiV,
                                     unsigned PaV,
                                     CoreGenErrno *E)
  : Name(N), NumFeatures(0), HDLCodegen(HDL), LLVMCodegen(LLVM),
    MajorVersion(MaV), MinorVersion(MiV), PatchVersion(PaV),
    Errno(E),Top(new CoreGenNode(CGTop,Errno)) {
}

CoreGenPluginImpl::~CoreGenPluginImpl(){

  this->PurgeNodes();

  if( Top ){
    delete Top;
  }
}

bool CoreGenPluginImpl::PurgeNodes(){
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    Top->DeleteChild( Top->GetChild(i) );
  }
  for( unsigned i=0; i<Caches.size(); i++ ){
    delete Caches[i];
  }
  for( unsigned i=0; i<Cores.size(); i++ ){
    delete Cores[i];
  }
  for( unsigned i=0; i<Exts.size(); i++ ){
    delete Exts[i];
  }
  for( unsigned i=0; i<Insts.size(); i++ ){
    delete Insts[i];
  }
  for( unsigned i=0; i<PInsts.size(); i++ ){
    delete PInsts[i];
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
  for( unsigned i=0; i<Socs.size(); i++ ){
    delete Socs[i];
  }
  for( unsigned i=0; i<ISAs.size(); i++ ){
    delete ISAs[i];
  }
  for( unsigned i=0; i<Comms.size(); i++ ){
    delete Comms[i];
  }
  for( unsigned i=0; i<Spads.size(); i++ ){
    delete Spads[i];
  }
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    delete MCtrls[i];
  }
  for( unsigned i=0; i<VTPs.size(); i++ ){
    delete VTPs[i];
  }

  return true;
}

void CoreGenPluginImpl::GetVersion( unsigned *Major,
                                    unsigned *Minor,
                                    unsigned *Patch ){
  if( Major ){
    *Major = MajorVersion;
  }
  if( Minor ){
    *Minor = MinorVersion;
  }
  if( Patch ){
    *Patch = PatchVersion;
  }
}

bool CoreGenPluginImpl::InsertFeature(std::string Name,
                                      CGFeatureType Type,
                                      CGFeatureVal Value){

  FeatureTable.push_back({Name,Type,Value});
  NumFeatures = FeatureTable.size();
  return true;
}

CGFeatureType CoreGenPluginImpl::GetFeatureType( std::string Feature ){
  if( Feature.length() == 0 ){
    return CGFUnknown;
  }

  for( unsigned i=0; i<FeatureTable.size(); i++ ){
    if( FeatureTable[i].Name == Feature ){
      return FeatureTable[i].Type;
    }
  }

  return CGFUnknown;
}

CGFeatureType CoreGenPluginImpl::GetFeatureType( unsigned Idx ){
  if( Idx > (FeatureTable.size()-1) ){
    return CGFUnknown;
  }
  return FeatureTable[Idx].Type;
}

std::string CoreGenPluginImpl::GetFeatureName( unsigned Idx ){
  if( Idx > (FeatureTable.size()-1) ){
    return "";
  }
  return FeatureTable[Idx].Name;
}

CGFeatureVal CoreGenPluginImpl::GetFeatureValue( std::string Feature ){
  CGFeatureVal Val;
  if( Feature.length() == 0 ){
    return Val;
  }

  for( unsigned i=0; i<FeatureTable.size(); i++ ){
    if( FeatureTable[i].Name == Feature ){
      return FeatureTable[i].Value;
    }
  }
  return Val;
}

CGFeatureVal CoreGenPluginImpl::GetFeatureValue( unsigned Idx ){
  CGFeatureVal Val;
  if( Idx > (FeatureTable.size()-1) ){
    return Val;
  }
  return FeatureTable[Idx].Value;
}

bool CoreGenPluginImpl::SetFeatureValue( std::string Feature,
                                         CGFeatureVal Val ){
  for( unsigned i=0; i<FeatureTable.size(); i++ ){
    if( FeatureTable[i].Name == Feature ){
      FeatureTable[i].Value = Val;
      return true;
    }
  }
  return false;
}

bool CoreGenPluginImpl::SetFeatureValue( unsigned Idx,
                                         CGFeatureVal Val ){
  if( Idx > (FeatureTable.size()-1) ){
    return false;
  }
  FeatureTable[Idx].Value = Val;
  return true;
}

bool CoreGenPluginImpl::InsertNode( CoreGenNode *N ){
  if( !N ){
    return false;
  }
  Nodes.push_back(N);
  return true;
}

CoreGenCache *CoreGenPluginImpl::InsertCache(std::string Name,
                                          unsigned Sets,
                                          unsigned Ways){
  CoreGenCache *C = new CoreGenCache(Name,Sets,Ways,Errno);
  Caches.push_back(C);
  return C;
}

CoreGenCore *CoreGenPluginImpl::InsertCore(std::string Name,
                                        CoreGenISA *ISA ){
  CoreGenCore *C = new CoreGenCore(Name, ISA, Errno);
  Cores.push_back(C);
  return C;
}

CoreGenExt *CoreGenPluginImpl::InsertExt(std::string Name){
  CoreGenExt *E = new CoreGenExt(Name,Errno);
  Exts.push_back(E);
  return E;
}

CoreGenInst *CoreGenPluginImpl::InsertInst(std::string Name, CoreGenISA *ISA,
                                        CoreGenInstFormat *Format ){
  CoreGenInst *I = new CoreGenInst(Name,ISA,Format,Errno);
  Insts.push_back(I);
  return I;
}

CoreGenPseudoInst *CoreGenPluginImpl::InsertPseudoInst(std::string Name,
                                                    CoreGenInst *Inst ){
  CoreGenPseudoInst *P = new CoreGenPseudoInst(Name,Inst,Errno);
  PInsts.push_back(P);
  return P;
}

CoreGenInstFormat *CoreGenPluginImpl::InsertInstFormat(std::string Name,
                                                    CoreGenISA *ISA){
  CoreGenInstFormat *IF = new CoreGenInstFormat(Name,ISA,Errno);
  Formats.push_back(IF);
  return IF;
}

CoreGenReg *CoreGenPluginImpl::InsertReg(std::string Name,
                                      int RegIndex,
                                      int RegWidth ){
  CoreGenReg *R = new CoreGenReg(Name,RegIndex,RegWidth,Errno);
  Regs.push_back(R);
  return R;
}

CoreGenRegClass *CoreGenPluginImpl::InsertRegClass(std::string Name){
  CoreGenRegClass *RC = new CoreGenRegClass(Name,Errno);
  RegClasses.push_back(RC);
  return RC;
}

CoreGenSoC *CoreGenPluginImpl::InsertSoC(std::string Name){
  CoreGenSoC *S = new CoreGenSoC(Name,Errno);
  Socs.push_back(S);
  return S;
}

CoreGenISA *CoreGenPluginImpl::InsertISA(std::string Name){
  CoreGenISA *I = new CoreGenISA(Name,Errno);
  ISAs.push_back(I);
  return I;
}

CoreGenComm *CoreGenPluginImpl::InsertComm(std::string Name){
  CoreGenComm *C = new CoreGenComm(Name,Errno);
  Comms.push_back(C);
  return C;
}

void CoreGenPluginImpl::InitFeatureValue(CGFeatureVal &Value){
  Value.UnsignedData = 0;
  Value.Uint32tData = 0x00;
  Value.Int32tData = 0x00;
  Value.Uint64tData = 0x00ull;
  Value.Int64tData = 0x00ll;
  Value.FloatData = 0.;
  Value.DoubleData = 0.;
  Value.StringData = "";
  Value.BoolData = false;
}

bool CoreGenPluginImpl::BuildDAG(){
  // purge all the children of Top
  Top->PurgeChildren();

  // add all the subnodes
  for( unsigned i=0; i<Caches.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Caches[i]));
  }
  for( unsigned i=0; i<Cores.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Cores[i]));
  }
  for( unsigned i=0; i<Insts.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Insts[i]));
  }
  for( unsigned i=0; i<PInsts.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(PInsts[i]));
  }
  for( unsigned i=0; i<Formats.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Formats[i]));
  }
  for( unsigned i=0; i<Regs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Regs[i]));
  }
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(RegClasses[i]));
  }
  for( unsigned i=0; i<Socs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Socs[i]));
  }
  for( unsigned i=0; i<ISAs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(ISAs[i]));
  }
  for( unsigned i=0; i<Exts.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Exts[i]));
  }
  for( unsigned i=0; i<Comms.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Comms[i]));
  }
  for( unsigned i=0; i<Spads.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Spads[i]));
  }
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(MCtrls[i]));
  }
  for( unsigned i=0; i<VTPs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(VTPs[i]));
  }
  return true;
}

// EOF
