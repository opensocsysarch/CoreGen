//
// _CoreGenBackend_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

CoreGenBackend::CoreGenBackend() :
  Errno(new CoreGenErrno()),
  Proj(nullptr),
  Env(new CoreGenEnv(Errno)),
  PluginMgr(new CoreGenPluginMgr(Errno)),
  PassMgr(nullptr),
  Top(new CoreGenNode(CGTop,Errno)),
  DAG(nullptr){
}

CoreGenBackend::CoreGenBackend(std::string ProjName,
                               std::string ProjRoot,
                               std::string ArchRoot ) :
  Errno(new CoreGenErrno()),
  Proj(new CoreGenProj(Errno,ProjName,ProjRoot)),
  Env(new CoreGenEnv(ArchRoot,Errno)),
  PluginMgr(new CoreGenPluginMgr(Errno)),
  PassMgr(nullptr),
  Top(new CoreGenNode(CGTop,Errno)),
  DAG(nullptr){
}

CoreGenBackend::CoreGenBackend(std::string ProjName,
                               std::string ProjRoot,
                               std::string ArchRoot,
                               CGProjType Type) :
  Errno(new CoreGenErrno()),
  Proj(new CoreGenProj(Errno,ProjName,ProjRoot,Type)),
  Env(new CoreGenEnv(ArchRoot,Errno)),
  PluginMgr(new CoreGenPluginMgr(Errno)),
  PassMgr(nullptr),
  Top(new CoreGenNode(CGTop,Errno)),
  DAG(nullptr) {
}

CoreGenBackend::~CoreGenBackend(){
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
  for( unsigned i=0; i<Plugins.size(); i++ ){
    delete Plugins[i];
  }


  delete Top;
  delete PluginMgr;
  if( PassMgr != nullptr ){
    delete PassMgr;
  }
  if( DAG != nullptr ){
    delete DAG;
  }
  if( Proj != nullptr ){
    delete Proj;
  }
  if( Env != nullptr ){
    delete Env;
  }
  delete Errno;
}

bool CoreGenBackend::ExecuteLLVMCodegen(){
  // Create the codegen object
  CoreGenCodegen *CG = new CoreGenCodegen(Top,
                                          Proj,
                                          Errno);

  if( CG == nullptr ){
    Errno->SetError(CGERR_ERROR, "Could not create codegen object");
    return false;
  }

  // Execute it
  bool rtn = CG->ExecuteLLVMCodegen();

  // delete and clean everything up
  delete CG;
  return rtn;
}

bool CoreGenBackend::IsValidName(std::string N){
  // YAML Node Name Rules:
  // - must have a length > 0
  // - must start with an alpha character
  // - must only contain alphanumeric characters, except '.'

  if( N.length() == 0 ){
    return false;
  }
  if( !std::isalpha(N[0]) ){
    return false;
  }
  for( unsigned i=0; i<N.length(); i++ ){
    if( N[i] != '.' ){
      if( !std::isalnum(N[i]) ){
        return false;
      }
    }
  }
  return true;
}

bool CoreGenBackend::ExecuteChiselCodegen(){
  // Create the codegen object
  CoreGenCodegen *CG = new CoreGenCodegen(Top,
                                          Proj,
                                          Errno);

  if( CG == nullptr ){
    Errno->SetError(CGERR_ERROR, "Could not create codegen object");
    return false;
  }

  // Execute it
  bool rtn = CG->ExecuteChiselCodegen();

  // delete and clean everything up
  delete CG;
  return rtn;
}

bool CoreGenBackend::ExecuteCodegen(){
  // Create the codegen object
  CoreGenCodegen *CG = new CoreGenCodegen(Top,
                                          Proj,
                                          Errno);

  if( CG == nullptr ){
    Errno->SetError(CGERR_ERROR, "Could not create codegen object");
    return false;
  }

  // Execute it
  bool rtn = CG->Execute();

  // delete and clean everything up
  delete CG;
  return rtn;
}

bool CoreGenBackend::ExecutePasses(){
  if( PassMgr == nullptr ){
    Errno->SetError(CGERR_ERROR, "PassMgr has not been enabled");
    return false;
  }
  return PassMgr->Execute();
}

bool CoreGenBackend::ExecutePass( std::string str ){
  if( PassMgr == nullptr ){
    Errno->SetError(CGERR_ERROR, "PassMgr has not been enabled");
    return false;
  }
  return PassMgr->Execute(str);
}

bool CoreGenBackend::ExecuteSysPass( std::string str ){
  if( PassMgr == nullptr ){
    Errno->SetError(CGERR_ERROR, "PassMgr has not been enabled");
    return false;
  }
  return PassMgr->ExecuteSys(str);
}

bool CoreGenBackend::SetPassOutputPath( std::string Pass, std::string Path ){
  if( PassMgr == nullptr ){
    Errno->SetError(CGERR_ERROR, "PassMgr has not been enabled");
    return false;
  }
  return PassMgr->SetPassOutputPath(Pass,Path);
}

bool CoreGenBackend::SetPassInputStr( std::string Pass, std::string Str ){
  if( PassMgr == nullptr ){
    Errno->SetError(CGERR_ERROR, "PassMgr has not been enabled");
    return false;
  }
  return PassMgr->SetPassInputStr(Pass,Str);
}

bool CoreGenBackend::ExecutePassType(CGPassType T ){
  if( PassMgr == nullptr ){
    Errno->SetError(CGERR_ERROR, "PassMgr has not been enabled");
    return false;
  }
  return PassMgr->Execute(T);
}

void CoreGenBackend::CoreGenVersion( int *major, int *minor ){
  *major = CoreGenBackend_VERSION_MAJOR;
  *minor = CoreGenBackend_VERSION_MINOR;
}

bool CoreGenBackend::WriteIR( std::string FileName ){
  if( FileName.length() == 0 ){
    Errno->SetError( CGERR_ERROR, "FileName is empty" );
    return false;
  }

  CoreGenYaml *IR = new CoreGenYaml( FileName, PluginMgr, Env, Proj, Errno );
  if( !IR ){
    Errno->SetError( CGERR_ERROR, "Failed to create Yaml IR object" );
    return false;
  }

  if( !IR->WriteYaml( Socs, Cores, Caches, ISAs, Formats, Insts,
                      PInsts, RegClasses, Regs, Comms, Spads,
                      MCtrls, VTPs, Exts, Plugins) ){
    delete IR;
    return false;
  }

  delete IR;

  return true;
}

bool CoreGenBackend::ReadIR( std::string FileName ){
  if( FileName.length() == 0 ){
    Errno->SetError( CGERR_ERROR, "FileName is empty" );
    return false;
  }

  if( Proj == nullptr ){
    Proj = new CoreGenProj(Errno);
  }

  CoreGenYaml *IR = new CoreGenYaml( FileName, PluginMgr, Env, Proj, Errno );
  if( !IR ){
    Errno->SetError( CGERR_ERROR, "Failed to create Yaml IR object" );
    return false;
  }

  if( !IR->ReadYaml( Socs, Cores, Caches, ISAs, Formats, Insts,
                      PInsts, RegClasses, Regs, Comms, Spads,
                      MCtrls, VTPs, Exts, Plugins) ){
    delete IR;
    return false;
  }

  delete IR;

  return true;
}

bool CoreGenBackend::LoadPlugin( std::string Path ){
  if( !PluginMgr ){
    return false;
  }
  return PluginMgr->LoadPlugin( Path );
}

bool CoreGenBackend::ReleasePlugin( unsigned Idx ){
  if( !PluginMgr ){
    return false;
  }
  return PluginMgr->ReleasePlugin(Idx);
}

bool CoreGenBackend::ReleasePlugin( std::string Plugin ){
  if( !PluginMgr ){
    return false;
  }
  return PluginMgr->ReleasePlugin(Plugin);
}

unsigned CoreGenBackend::GetNumPlugins(){
  if( !PluginMgr ){
    return 0;
  }
  return PluginMgr->GetNumPlugins();
}

std::string CoreGenBackend::GetErrStr(){
  if( !Errno ){
    return "Error in utilizing Errno";
  }
  return Errno->GetErrStr();
}

bool CoreGenBackend::ExportDOT( std::string FName ){
  if( DAG == nullptr ){
    return false;
  }
  return DAG->ExportDOT(FName);
}

std::vector<std::string> CoreGenBackend::GetPasses(){
  std::vector<std::string> dummy;
  if( PassMgr == nullptr ){
    Errno->SetError( CGERR_ERROR, "Pass manager is null" );
    return dummy;
  }
  return PassMgr->GetPassNames();
}

std::vector<std::string> CoreGenBackend::GetPassDescriptions(){
  std::vector<std::string> dummy;
  if( PassMgr == nullptr ){
    Errno->SetError( CGERR_ERROR, "Pass manager is null" );
    return dummy;
  }
  return PassMgr->GetPassDescriptions();
}

bool CoreGenBackend::PrintPassInfo(){
  if( PassMgr == nullptr ){
    Errno->SetError( CGERR_ERROR, "Pass manager is null" );
    return false;
  }
  return PassMgr->PrintPassInfo();
}

bool CoreGenBackend::PrintSysPassInfo(){
  if( PassMgr == nullptr ){
    Errno->SetError( CGERR_ERROR, "Pass manager is null" );
    return false;
  }
  return PassMgr->PrintSysPassInfo();
}

bool CoreGenBackend::IsPassMgr(){
  if( PassMgr != nullptr ){
    return true;
  }else{
    return false;
  }
}

bool CoreGenBackend::InitPassMgr(std::ostream *O){
  if( DAG == nullptr ){
    Errno->SetError( CGERR_ERROR, "IR DAG has not been constructed" );
    return false;
  }
  if( Proj == nullptr ){
    Errno->SetError( CGERR_ERROR, "The project info is missing" );
    return false;
  }
  if( PassMgr ){
    return true;
  }
  PassMgr = new CoreGenPassMgr(O,Errno, DAG, Proj);
  if( PassMgr == nullptr ){
    return false;
  }
  return true;
}

bool CoreGenBackend::InitPassMgr(){
  if( DAG == nullptr ){
    Errno->SetError( CGERR_ERROR, "IR DAG has not been constructed" );
    return false;
  }
  if( Proj == nullptr ){
    Errno->SetError( CGERR_ERROR, "The project info is missing" );
    return false;
  }
  std::ostream *o = &std::cout;
  PassMgr = new CoreGenPassMgr(o,Errno, DAG, Proj);
  if( PassMgr == nullptr ){
    return false;
  }
  return true;
}

bool CoreGenBackend::PurgeDAG(){
#if 0
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( !Top->DeleteChild(Top->GetChild(i)) ){
      Errno->SetError( CGERR_ERROR, "Error purging top-level nodes" );
      return false;
    }
  }
#endif
  Top->PurgeChildren();
  return true;
}

bool CoreGenBackend::BuildDAG(){
  if( DAG != nullptr ){
    // delete the old DAG
    this->PurgeDAG();
    //delete Top;
    delete DAG;
    //Top = new CoreGenNode(CGTop,Errno);
  }

  DAG = new CoreGenDAG(Errno);
  if( DAG == nullptr ){
    Errno->SetError( CGERR_ERROR, "Error allocating new top-level DAG" );
    return false;
  }

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
  for( unsigned i=0; i<Plugins.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Plugins[i]));
    Plugins[i]->BuildDAG();
  }

  // build the L0 DAG
  if( !DAG->BuildDAG(Top) ){
    Errno->SetError( CGERR_ERROR, "Error building L0 DAG" );
    return false;
  }

  // Lower everything
  return DAG->LowerAll();
}

bool CoreGenBackend::DeleteDepChild( CoreGenNode *N ){
  // delete all the dependent child nodes
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    for( unsigned j=0; j<Top->GetChild(i)->GetNumChild(); j++ ){
      if( Top->GetChild(i)->GetChild(j) == N ){
        Top->GetChild(i)->DeleteChild(Top->GetChild(i)->GetChild(j));
      }
    }
  }

  // delete all the comm references
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGComm ){
      // found a comm node
      CoreGenComm *Comm = static_cast<CoreGenComm *>(Top->GetChild(i));
      for( unsigned j=0; j<Comm->GetNumEndpoints(); j++ ){
        if( Comm->GetEndpoint(j) == N ){
          Comm->DeleteEndpoint(j);
        }
      }
    }
  }

  return true;
}

bool CoreGenBackend::DeleteCacheNode(CoreGenCache *C){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(C) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(C) );

  // stage 3: clean up the parent/child pointers
  for( unsigned i=0; i<Top->GetNumChild(); i++  ){
    if( Top->GetChild(i)->GetType() == CGCache ){
      CoreGenCache *TCache = static_cast<CoreGenCache *>(Top->GetChild(i));
      if( TCache->GetSubCache() == C ){
        TCache->SetNullChildCache();
      }
      if( TCache->HasParentCache(C) ){
        TCache->SetNullParentCache();
      }
    }
    if( Top->GetChild(i)->GetType() == CGCore ){
      CoreGenCore *TCore = static_cast<CoreGenCore *>(Top->GetChild(i));
      if( TCore->GetCache() == C ){
        TCore->SetNullCache();
      }
    }
  }

  // stgae 4: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Caches.size(); i++ ){
    if( Caches[i] == C )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Caches.erase(Caches.begin()+DIdx[i]);
  }

  delete C;

  return true;
}

bool CoreGenBackend::DeleteCoreNode(CoreGenCore *C){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(C) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }else if( Top->GetChild(i)->GetType() == CGSoc ){
      CoreGenSoC *Soc = static_cast<CoreGenSoC *>(Top->GetChild(i));
      for( unsigned i=0; i<Soc->GetNumCores(); i++ ){
        if( Soc->GetCore(i) == C ){
          Soc->DeleteCore( C );
        }
      }
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(C) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Cores.size(); i++ ){
    if( Cores[i] == C )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Cores.erase(Cores.begin()+DIdx[i]);
  }

  delete C;

  return true;
}

bool CoreGenBackend::DeleteInstNode(CoreGenInst *Inst){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(Inst) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(Inst) );

  for( unsigned i=0; i<Top->GetNumChild(); i++  ){
    if( Top->GetChild(i)->GetType() == CGPInst ){
      CoreGenPseudoInst *PInst = static_cast<CoreGenPseudoInst *>(Top->GetChild(i));
      if( PInst->GetInst() == Inst ){
        PInst->SetNullInst();
      }
    }
  }

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Insts.size(); i++ ){
    if( Insts[i] == Inst )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Insts.erase(Insts.begin()+DIdx[i]);
  }

  delete Inst;

  return true;
}

bool CoreGenBackend::DeletePInstNode(CoreGenPseudoInst *P){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(P) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(P) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<PInsts.size(); i++ ){
    if( PInsts[i] == P )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    PInsts.erase(PInsts.begin()+DIdx[i]);
  }

  delete P;

  return true;
}

bool CoreGenBackend::DeleteInstFormatNode(CoreGenInstFormat *I){
  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(I) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(I) );

  // stage 3: walk all the instructions and nullify any candidates
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGInst ){
      CoreGenInst *Inst = static_cast<CoreGenInst *>(Top->GetChild(i));
      if( Inst->GetFormat() == I ){
        Inst->SetNullFormat();
      }
    }
  }

  // stage 4: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Formats.size(); i++ ){
    if( Formats[i] == I )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Formats.erase(Formats.begin()+DIdx[i]);
  }

  delete I;

  return true;
}

bool CoreGenBackend::DeleteRegNode(CoreGenReg *R){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(R) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(R) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Regs.size(); i++ ){
    if( Regs[i] == R )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Regs.erase(Regs.begin()+DIdx[i]);
  }

  delete R;

  return true;
}

bool CoreGenBackend::DeleteRegClassNode(CoreGenRegClass *RC){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(RC) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(RC) );

  // stage 3: remove the register class attributes from any instructions
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() ==  CGInstF ){
      // get the instruction format
      CoreGenInstFormat *Format =
        static_cast<CoreGenInstFormat *>(Top->GetChild(i));

      for( unsigned j=0; j<Format->GetNumFields(); j++ ){
        if( (Format->GetFieldType(Format->GetFieldName(j)) ==
            CoreGenInstFormat::CGInstReg) &&
            (Format->GetFieldRegClass(Format->GetFieldName(j)) == RC) ){
          Format->SetNullField(Format->GetFieldName(j));
        }
      }
    }
    if( Top->GetChild(i)->GetType() ==  CGCore ){
      CoreGenCore *Core = static_cast<CoreGenCore *>(Top->GetChild(i));
      for( unsigned j=0; j<Core->GetNumRegClass(); j++ ){
        if( Core->GetRegClass(j) == RC ){
          Core->DeleteRegClass(j);
        }
      }
    }
  }

  // stage 4: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    if( RegClasses[i] == RC )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    RegClasses.erase(RegClasses.begin()+DIdx[i]);
  }

  delete RC;

  return true;
}

bool CoreGenBackend::DeleteSoCNode(CoreGenSoC *S){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(S) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(S) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Socs.size(); i++ ){
    if( Socs[i] == S )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Socs.erase(Socs.begin()+DIdx[i]);
  }

  delete S;

  return true;
}

bool CoreGenBackend::DeleteISANode(CoreGenISA *ISA){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(ISA) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(ISA) );

  // stage 3: walk all the nodes and set any instructions ISA to null
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGInst ){
      CoreGenInst *Inst = static_cast<CoreGenInst *>(Top->GetChild(i));
      if( Inst->GetISA() == ISA ){
        Inst->SetNullISA();
      }
    }
    if( Top->GetChild(i)->GetType() == CGPInst ){
      CoreGenPseudoInst *PInst = static_cast<CoreGenPseudoInst *>(Top->GetChild(i));
      if( PInst->GetISA() == ISA ){
        PInst->SetNullISA();
      }
    }
    if( Top->GetChild(i)->GetType() == CGCore ){
      CoreGenCore *Core = static_cast<CoreGenCore *>(Top->GetChild(i));
      if( Core->GetISA() == ISA ){
        Core->SetNullISA();
      }
    }
  }

  // stage 4: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<ISAs.size(); i++ ){
    if( ISAs[i] == ISA )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    ISAs.erase(ISAs.begin()+DIdx[i]);
  }

  delete ISA;

  return true;
}

bool CoreGenBackend::DeleteExtNode(CoreGenExt *E){
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(E) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(E) );

  // NOTE: when deleting extensions, it is not currently possible to know
  // the difference between an existing node and a new node that was inserted
  // specifically for this extension.  As a result, all the child nodes will
  // become orphans

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Exts.size(); i++ ){
    if( Exts[i] == E )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Exts.erase(Exts.begin()+DIdx[i]);
  }

  delete E;

  return true;
}

bool CoreGenBackend::DeleteCommNode(CoreGenComm *C){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(C) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(C) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Comms.size(); i++ ){
    if( Comms[i] == C )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Comms.erase(Comms.begin()+DIdx[i]);
  }

  delete C;

  return true;
}

bool CoreGenBackend::DeleteSpadNode(CoreGenSpad *S){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(S) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(S) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Spads.size(); i++ ){
    if( Spads[i] == S )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Spads.erase(Spads.begin()+DIdx[i]);
  }

  delete S;

  return true;
}

bool CoreGenBackend::DeleteMCtrlNode(CoreGenMCtrl *M){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(M) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(M) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    if( MCtrls[i] == M )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    MCtrls.erase(MCtrls.begin()+DIdx[i]);
  }

  delete M;

  return true;
}

bool CoreGenBackend::DeleteVTPNode(CoreGenVTP *V){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(V) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk all the nodes and determine if anyone is pointing to us
  DeleteDepChild( static_cast<CoreGenNode *>(V) );

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<VTPs.size(); i++ ){
    if( VTPs[i] == V )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    VTPs.erase(VTPs.begin()+DIdx[i]);
  }

  delete V;

  return true;
}

bool CoreGenBackend::DeletePluginNode(CoreGenPlugin *P){

  // stage 1: walk all the top-level nodes and ensure that we remove the node
  if( !DAG )
    this->BuildDAG();

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(P) ==
        Top->GetChild(i) ){
      Top->DeleteChild(Top->GetChild(i));
    }
  }

  // stage 2: walk the nodes and determine if anyone is using
  //          the target node as a overriden plugin, if so, remove
  //          that node and adjust any links to it
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( static_cast<CoreGenNode *>(P) ==
        Top->GetChild(i)->GetPlugin() ){
      // adjust the links pointing to us
      for( unsigned j=0; j<Top->GetNumChild(); j++ ){
        for( unsigned k=0; k<Top->GetChild(j)->GetNumChild(); k++ ){
          if( Top->GetChild(j)->GetChild(k) == Top->GetChild(i) ){
            Top->GetChild(j)->DeleteChild(Top->GetChild(i));
          }
        }
      }
    }
  }

  // stage 3: remove from the vector
  std::vector<unsigned> DIdx;
  for( unsigned i=0; i<Plugins.size(); i++ ){
    if( Plugins[i] == P )
      DIdx.push_back(i);
  }
  for( unsigned i=0; i<DIdx.size(); i++ ){
    Plugins.erase(Plugins.begin()+DIdx[i]);
  }

  // stage 4: delete the node
  delete P;

  return true;
}

bool CoreGenBackend::DeleteNode( CoreGenNode *N ){
  if( !N )
    return false;

  // stage 1: Determine what type of node this is
  //          Certain nodes will require deleting
  //          child nodes that are encodings.
  //          Further, we must remove the necessary
  //          nodes from the Top-level DAG lists
  switch(N->GetType()){
  case CGSoc:
    return DeleteSoCNode(static_cast<CoreGenSoC *>(N));
    break;
  case CGCore:
    return DeleteCoreNode(static_cast<CoreGenCore *>(N));
    break;
  case CGInstF:
    return DeleteInstFormatNode(static_cast<CoreGenInstFormat *>(N));
    break;
  case CGInst:
    return DeleteInstNode(static_cast<CoreGenInst *>(N));
    break;
  case CGPInst:
    return DeletePInstNode(static_cast<CoreGenPseudoInst *>(N));
    break;
  case CGRegC:
    return DeleteRegClassNode(static_cast<CoreGenRegClass *>(N));
    break;
  case CGReg:
    return DeleteRegNode(static_cast<CoreGenReg *>(N));
    break;
  case CGISA:
    return DeleteISANode(static_cast<CoreGenISA *>(N));
    break;
  case CGCache:
    return DeleteCacheNode(static_cast<CoreGenCache *>(N));
    break;
  case CGEnc:
    Errno->SetError( CGERR_ERROR, "Cannot delete individual encodings" );
    return false;
    break;
  case CGExt:
    return DeleteExtNode(static_cast<CoreGenExt *>(N));
    break;
  case CGComm:
    return DeleteCommNode(static_cast<CoreGenComm *>(N));
    break;
  case CGSpad:
    return DeleteSpadNode(static_cast<CoreGenSpad *>(N));
    break;
  case CGMCtrl:
    return DeleteMCtrlNode(static_cast<CoreGenMCtrl *>(N));
    break;
  case CGVTP:
    return DeleteVTPNode(static_cast<CoreGenVTP *>(N));
    break;
  case CGPlugin:
    Errno->SetError( CGERR_WARN, "Deleting a plugin will remove all its children" );
    return DeletePluginNode(static_cast<CoreGenPlugin *>(N));
    break;
  case CGTop:
    Errno->SetError( CGERR_ERROR, "Top-level DAG nodes cannot be deleted" );
    return false;
    break;
  default:
    // something went wrong
    Errno->SetError( CGERR_ERROR, "Unrecognized node type for deletion" );
    delete N;
    return false;
    break;
  }

  // if we get here, something went wrong
  return false;
}

CoreGenCache *CoreGenBackend::InsertCache(std::string Name,
                                          unsigned Sets,
                                          unsigned Ways){
  CoreGenCache *C = new CoreGenCache(Name,Sets,Ways,Errno);
  Caches.push_back(C);
  return C;
}

CoreGenCore *CoreGenBackend::InsertCore(std::string Name,
                                        CoreGenISA *ISA ){
  CoreGenCore *C = new CoreGenCore(Name, ISA, Errno);
  Cores.push_back(C);
  return C;
}

CoreGenExt *CoreGenBackend::InsertExt(std::string Name){
  CoreGenExt *E = new CoreGenExt(Name,Errno);
  Exts.push_back(E);
  return E;
}

CoreGenInst *CoreGenBackend::InsertInst(std::string Name, CoreGenISA *ISA,
                                        CoreGenInstFormat *Format ){
  CoreGenInst *I = new CoreGenInst(Name,ISA,Format,Errno);
  Insts.push_back(I);
  return I;
}

CoreGenPseudoInst *CoreGenBackend::InsertPseudoInst(std::string Name,
                                                    CoreGenInst *Inst ){
  CoreGenPseudoInst *P = new CoreGenPseudoInst(Name,Inst,Errno);
  PInsts.push_back(P);
  return P;
}

CoreGenInstFormat *CoreGenBackend::InsertInstFormat(std::string Name,
                                                    CoreGenISA *ISA){
  CoreGenInstFormat *IF = new CoreGenInstFormat(Name,ISA,Errno);
  Formats.push_back(IF);
  return IF;
}

CoreGenReg *CoreGenBackend::InsertReg(std::string Name,
                                      int RegIndex,
                                      int RegWidth ){
  CoreGenReg *R = new CoreGenReg(Name,RegIndex,RegWidth,Errno);
  Regs.push_back(R);
  return R;
}

CoreGenRegClass *CoreGenBackend::InsertRegClass(std::string Name){
  CoreGenRegClass *RC = new CoreGenRegClass(Name,Errno);
  RegClasses.push_back(RC);
  return RC;
}

CoreGenSoC *CoreGenBackend::InsertSoC(std::string Name){
  CoreGenSoC *S = new CoreGenSoC(Name,Errno);
  Socs.push_back(S);
  return S;
}

CoreGenISA *CoreGenBackend::InsertISA(std::string Name){
  CoreGenISA *I = new CoreGenISA(Name,Errno);
  ISAs.push_back(I);
  return I;
}

CoreGenComm *CoreGenBackend::InsertComm(std::string Name){
  CoreGenComm *C = new CoreGenComm(Name,Errno);
  Comms.push_back(C);
  return C;
}

CoreGenSpad *CoreGenBackend::InsertSpad(std::string Name,
                                        unsigned MemSize,
                                        unsigned RqstPorts,
                                        unsigned RspPorts){
  CoreGenSpad *S = new CoreGenSpad(Name,Errno,MemSize,RqstPorts,RspPorts);
  Spads.push_back(S);
  return S;
}

CoreGenMCtrl *CoreGenBackend::InsertMCtrl(std::string Name,
                                          unsigned Ports){
  CoreGenMCtrl *M = new CoreGenMCtrl(Name,Errno,Ports);
  MCtrls.push_back(M);
  return M;
}

CoreGenVTP *CoreGenBackend::InsertVTP(std::string Name){
  CoreGenVTP *V = new CoreGenVTP(Name,Errno);
  VTPs.push_back(V);
  return V;
}

bool CoreGenBackend::InsertPlugin( CoreGenPlugin *Plugin ){
  if( !Plugin ){
    return false;
  }
  Plugins.push_back(Plugin);
  return true;
}

CoreGenPlugin *CoreGenBackend::GetPlugin( unsigned P ){
  if( !PluginMgr ){
    return nullptr;
  }
  return PluginMgr->GetPlugin(P);
}

CoreGenPlugin *CoreGenBackend::GetPlugin( std::string Plugin ){
  if( !PluginMgr ){
    return nullptr;
  }
  return PluginMgr->GetPlugin(Plugin);
}

CoreGenCache* CoreGenBackend::GetCacheNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Caches.size(); i++){
    if(this->Caches[i]->GetName() == Name){
      return this->Caches[i];
    }
  }
  return NULL;
}

CoreGenCore* CoreGenBackend::GetCoreNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Cores.size(); i++){
    if(this->Cores[i]->GetName() == Name){
      return this->Cores[i];
    }
  }
  return NULL;
}

CoreGenInst* CoreGenBackend::GetInstNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Insts.size(); i++){
    if(this->Insts[i]->GetName() == Name){
      return this->Insts[i];
    }
  }
  return NULL;
}

CoreGenPseudoInst* CoreGenBackend::GetPInstNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->PInsts.size(); i++){
    if(this->PInsts[i]->GetName() == Name){
      return this->PInsts[i];
    }
  }
  return NULL;
}

CoreGenPseudoInst* CoreGenBackend::GetPInstNodeByInstName(std::string Name){
  for(unsigned int i = 0; i < this->PInsts.size(); i++){
    if(this->PInsts[i]->GetInst()->GetName() == Name){
      return this->PInsts[i];
    }
  }
  return NULL;
}

CoreGenInstFormat* CoreGenBackend::GetInstFormatNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Formats.size(); i++){
    if(this->Formats[i]->GetName() == Name){
      return this->Formats[i];
    }
  }
  return NULL;
}

CoreGenReg* CoreGenBackend::GetRegNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Regs.size(); i++){
    if(this->Regs[i]->GetName() == Name){
      return this->Regs[i];
    }
  }
  return NULL;
}

CoreGenRegClass* CoreGenBackend::GetRegClassNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->RegClasses.size(); i++){
    if(this->RegClasses[i]->GetName() == Name){
      return this->RegClasses[i];
    }
  }
  return NULL;
}

CoreGenSoC* CoreGenBackend::GetSoCNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Socs.size(); i++){
    if(this->Socs[i]->GetName() == Name){
      return this->Socs[i];
    }
  }
  return NULL;
}

CoreGenISA* CoreGenBackend::GetISANodeByName(std::string Name){
  for(unsigned int i = 0; i < this->ISAs.size(); i++){
    if(this->ISAs[i]->GetName() == Name){
      return this->ISAs[i];
    }
  }
  return NULL;
}

CoreGenExt* CoreGenBackend::GetExtNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Exts.size(); i++){
    if(this->Exts[i]->GetName() == Name){
      return this->Exts[i];
    }
  }
  return NULL;
}

CoreGenComm* CoreGenBackend::GetCommNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Comms.size(); i++){
    if(this->Comms[i]->GetName() == Name){
      return this->Comms[i];
    }
  }
  return NULL;
}

CoreGenSpad* CoreGenBackend::GetSpadNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Spads.size(); i++){
    if(this->Spads[i]->GetName() == Name){
      return this->Spads[i];
    }
  }
  return NULL;
}

CoreGenMCtrl* CoreGenBackend::GetMCtrlNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->MCtrls.size(); i++){
    if(this->MCtrls[i]->GetName() == Name){
      return this->MCtrls[i];
    }
  }
  return NULL;
}

CoreGenVTP* CoreGenBackend::GetVTPNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->VTPs.size(); i++){
    if(this->VTPs[i]->GetName() == Name){
      return this->VTPs[i];
    }
  }
  return NULL;
}

CoreGenPlugin* CoreGenBackend::GetPluginNodeByName(std::string Name){
  for(unsigned int i = 0; i < this->Plugins.size(); i++){
    if(this->Plugins[i]->GetName() == Name){
      return this->Plugins[i];
    }
  }
  return NULL;
}

CoreGenNode* CoreGenBackend::GetNodeByName( std::string Name ){

  // stage 1: Determine what type of node this is
  //          Certain nodes will require deleting
  //          child nodes that are encodings.
  //          Further, we must remove the necessary
  //          nodes from the Top-level DAG lists
  CoreGenNode *N;
  N = GetSoCNodeByName(Name);
  if(N) return N;
  N = GetCoreNodeByName(Name);
  if(N) return N;
  N = GetInstFormatNodeByName(Name);
  if(N) return N;
  N = GetInstNodeByName(Name);
  if(N) return N;
  N = GetPInstNodeByName(Name);
  if(N) return N;
  N = GetRegClassNodeByName(Name);
  if(N) return N;
  N = GetRegNodeByName(Name);
  if(N) return N;
  N = GetISANodeByName(Name);
  if(N) return N;
  N = GetCacheNodeByName(Name);
  if(N) return N;
  /*
  case CGEnc:
    Errno->SetError( CGERR_ERROR, "Cannot delete individual encodings" );
    return nullptr;
    break;
  */
  N = GetExtNodeByName(Name);
  if(N) return N;
  N = GetCommNodeByName(Name);
  if(N) return N;
  N = GetSpadNodeByName(Name);
  if(N) return N;
  N = GetMCtrlNodeByName(Name);
  if(N) return N;
  N = GetVTPNodeByName(Name);
  if(N) return N;
  N = GetPluginNodeByName(Name);
  if(N) return N;
  /*
  case CGTop:
    Errno->SetError( CGERR_ERROR, "Top-level DAG nodes cannot be deleted" );
    return nullptr;
    break;
  default:
    // something went wrong
    Errno->SetError( CGERR_ERROR, "Unrecognized node type for deletion" );
    delete N;
    return nullptr;
    break;
  }
  */

  // if we get here, something went wrong
  return nullptr;
}
// EOF
