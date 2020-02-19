//
// _CoreGenPlugin_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenPlugin.h"

CoreGenPlugin::CoreGenPlugin(std::string N,
                             CGPluginFunc F,
                             CoreGenEnv *Env,
                             CoreGenErrno *E)
  : CoreGenNode(CGPlugin,N,E), Funcs(F), Env(Env), Impl(nullptr) {
  create_t *create = Funcs.create;
  Impl = create(Env,E);
}

CoreGenPlugin::~CoreGenPlugin(){
  destroy_t *destroy = Funcs.destroy;
  destroy(Impl);
}

unsigned CoreGenPlugin::GetNumFeatures(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumFeatures();
}

bool CoreGenPlugin::HasHDLCodegen(){
  if( !Impl ){
    return false;
  }
  return Impl->HasHDLCodegen();
}

bool CoreGenPlugin::HasLLVMCodegen(){
  if( !Impl ){
    return false;
  }
  return Impl->HasLLVMCodegen();
}

void CoreGenPlugin::GetVersion( unsigned *Major,
                                unsigned *Minor,
                                unsigned *Patch ){
  if( !Impl ){
    return ;
  }
  if( !Major ){
    return ;
  }else if( !Minor ){
    return ;
  }else if( !Patch ){
    return ;
  }
  Impl->GetVersion(Major,Minor,Patch);
}

CGPluginFunc CoreGenPlugin::GetFunctionInfo(){
  return Funcs;
}

CoreGenPlugin *CoreGenPlugin::ClonePlugin(std::string NewName){
  return new CoreGenPlugin(NewName,
                           this->GetFunctionInfo(),
                           Env,
                           Errno);
}

std::string CoreGenPlugin::GetPluginName(){
  return Impl->GetName();
}

bool CoreGenPlugin::ProcessFeatures(){
  return Impl->ProcessFeatures();
}

CGFeatureType CoreGenPlugin::GetFeatureType( std::string Feature ){
  if( !Impl ){
    return CGFUnknown;
  }
  return Impl->GetFeatureType(Feature);
}

CGFeatureType CoreGenPlugin::GetFeatureType( unsigned Idx ){
  if( !Impl ){
    return CGFUnknown;
  }
  return Impl->GetFeatureType(Idx);
}

std::string CoreGenPlugin::GetFeatureName( unsigned Idx ){
  if( !Impl ){
    return "";
  }
  return Impl->GetFeatureName(Idx);
}

CGFeatureVal CoreGenPlugin::GetFeatureValue( std::string Feature ){
  CGFeatureVal NV;
  if( !Impl ){
    return NV;
  }
  return Impl->GetFeatureValue( Feature );
}

CGFeatureVal CoreGenPlugin::GetFeatureValue( unsigned Idx ){
  CGFeatureVal NV;
  if( !Impl ){
    return NV;
  }
  return Impl->GetFeatureValue( Idx );
}

bool CoreGenPlugin::SetFeatureValue( std::string Feature,
                                     CGFeatureVal Val ){
  if( !Impl ){
    return false;
  }
  return Impl->SetFeatureValue(Feature,Val);
}

bool CoreGenPlugin::SetFeatureValue( unsigned Idx,
                                     CGFeatureVal Val ){
  if( !Impl ){
    return false;
  }
  return Impl->SetFeatureValue( Idx, Val );
}

bool CoreGenPlugin::ExecuteHDLCodegen(CoreGenNode *TopNode,
                                      CoreGenNode *Parent){
  if( !Impl ){
    return false;
  }
  return Impl->ExecuteHDLCodegen(TopNode,Parent);
}

bool CoreGenPlugin::ExecuteLLVMCodegen(){
  if( !Impl ){
    return false;
  }
  return Impl->ExecuteLLVMCodegen();
}

CoreGenNode *CoreGenPlugin::GetTop(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetTop();
}

bool CoreGenPlugin::BuildDAG(){
  if( !Impl ){
    return false;
  }
  return Impl->BuildDAG();
}

CoreGenCache *CoreGenPlugin::InsertCache(std::string Name,
                                          unsigned Sets,
                                          unsigned Ways){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertCache(Name, Sets, Ways);
}

CoreGenCore *CoreGenPlugin::InsertCore(std::string Name,
                                        CoreGenISA *ISA ){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertCore(Name,ISA);
}

CoreGenExt *CoreGenPlugin::InsertExt(std::string Name){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertExt(Name);
}

CoreGenInst *CoreGenPlugin::InsertInst(std::string Name, CoreGenISA *ISA,
                                        CoreGenInstFormat *Format ){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertInst(Name,ISA,Format);
}

CoreGenPseudoInst *CoreGenPlugin::InsertPseudoInst(std::string Name,
                                                    CoreGenInst *Inst ){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertPseudoInst(Name,Inst);
}

CoreGenInstFormat *CoreGenPlugin::InsertInstFormat(std::string Name,
                                                    CoreGenISA *ISA){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertInstFormat(Name,ISA);
}

CoreGenReg *CoreGenPlugin::InsertReg(std::string Name,
                                      int RegIndex,
                                      int RegWidth ){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertReg(Name,RegIndex,RegWidth);
}

CoreGenRegClass *CoreGenPlugin::InsertRegClass(std::string Name){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertRegClass(Name);
}

CoreGenSoC *CoreGenPlugin::InsertSoC(std::string Name){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertSoC(Name);
}

CoreGenISA *CoreGenPlugin::InsertISA(std::string Name){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertISA(Name);
}

CoreGenComm *CoreGenPlugin::InsertComm(std::string Name){
  if( !Impl ){
    return nullptr;
  }
  return Impl->InsertComm(Name);
}

std::vector<CoreGenCache *> *CoreGenPlugin::GetCaches(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetCaches();
}

std::vector<CoreGenCore *> *CoreGenPlugin::GetCores(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetCores();
}

std::vector<CoreGenInst *> *CoreGenPlugin::GetInsts(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetInsts();
}

std::vector<CoreGenPseudoInst *> *CoreGenPlugin::GetPseudoInsts(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetPseudoInsts();
}

std::vector<CoreGenInstFormat *> *CoreGenPlugin::GetInstFormats(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetInstFormats();
}

std::vector<CoreGenReg *> *CoreGenPlugin::GetRegs(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetRegs();
}

std::vector<CoreGenRegClass *> *CoreGenPlugin::GetRegClasses(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetRegClasses();
}

std::vector<CoreGenSoC *> *CoreGenPlugin::GetSocs(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetSocs();
}

std::vector<CoreGenISA *> *CoreGenPlugin::GetISAs(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetISAs();
}

std::vector<CoreGenExt *> *CoreGenPlugin::GetExts(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetExts();
}

std::vector<CoreGenComm *> *CoreGenPlugin::GetComms(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetComms();
}

std::vector<CoreGenSpad *> *CoreGenPlugin::GetSpads(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetSpads();
}

std::vector<CoreGenMCtrl *> *CoreGenPlugin::GetMCtrls(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetMCtrls();
}

std::vector<CoreGenVTP *> *CoreGenPlugin::GetVTPs(){
  if( !Impl ){
    return nullptr;
  }
  return Impl->GetVTPs();
}

unsigned CoreGenPlugin::GetNumCaches(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumCaches();
}

unsigned CoreGenPlugin::GetNumCores(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumCores();
}

unsigned CoreGenPlugin::GetNumInsts(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumInsts();
}

unsigned CoreGenPlugin::GetNumPseudoInsts(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumPseudoInsts();
}

unsigned CoreGenPlugin::GetNumFormats(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumFormats();
}

unsigned CoreGenPlugin::GetNumRegs(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumRegs();
}

unsigned CoreGenPlugin::GetNumRegClasses(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumRegClasses();
}

unsigned CoreGenPlugin::GetNumSocs(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumSocs();
}

unsigned CoreGenPlugin::GetNumISAs(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumISAs();
}

unsigned CoreGenPlugin::GetNumExts(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumExts();
}

unsigned CoreGenPlugin::GetNumComms(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumComms();
}

unsigned CoreGenPlugin::GetNumSpads(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumSpads();
}

unsigned CoreGenPlugin::GetNumMCtrls(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumMCtrls();
}

unsigned CoreGenPlugin::GetNumVTPs(){
  if( !Impl ){
    return 0;
  }
  return Impl->GetNumVTPs();
}

std::vector<CoreGenCache *> &CoreGenPlugin::GetCacheVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetCacheVect();
}

std::vector<CoreGenInst *> &CoreGenPlugin::GetInstVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetInstVect();
}

std::vector<CoreGenExt *> &CoreGenPlugin::GetExtVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetExtVect();
}

std::vector<CoreGenPseudoInst *> &CoreGenPlugin::GetPseudoInstVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetPseudoInstVect();
}

std::vector<CoreGenInstFormat *> &CoreGenPlugin::GetInstFormatVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetInstFormatVect();
}

std::vector<CoreGenReg *> &CoreGenPlugin::GetRegVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetRegVect();
}

std::vector<CoreGenRegClass *> &CoreGenPlugin::GetRegClassVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetRegClassVect();
}

std::vector<CoreGenISA *> &CoreGenPlugin::GetISAVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetISAVect();
}

std::vector<CoreGenComm *> &CoreGenPlugin::GetCommVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetCommVect();
}

std::vector<CoreGenCore *> &CoreGenPlugin::GetCoreVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetCoreVect();
}

std::vector<CoreGenSpad *> &CoreGenPlugin::GetSpadVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetSpadVect();
}

std::vector<CoreGenMCtrl *> &CoreGenPlugin::GetMCtrlVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetMCtrlVect();
}

std::vector<CoreGenVTP *> &CoreGenPlugin::GetVTPVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetVTPVect();
}

std::vector<CoreGenSoC *> &CoreGenPlugin::GetSocVect(){
  if( !Impl ){
    Errno->SetError(CGERR_ERROR, "Plugin implementation not loaded" );
    throw "Plugin implementation not loaded";
  }
  return Impl->GetSocVect();
}

// EOF
