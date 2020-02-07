//
// _CoreGenPluginMgr_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenPluginMgr.h"

CoreGenPluginMgr::CoreGenPluginMgr( CoreGenEnv *EV, CoreGenErrno *E )
  : Env(EV), Errno(E){
}

CoreGenPluginMgr::~CoreGenPluginMgr(){
  for( unsigned i=0; i<Plugins.size(); i++ ){
    ReleasePlugin(i);
  }
}

#if (COREGEN_PLATFORM == Darwin) || (COREGEN_PLATFORM == Linux)
bool CoreGenPluginMgr::LoadNixPlugin( std::string Path ){

  void *handle;
  handle = dlopen( Path.c_str(), RTLD_NOW );
  if( !handle ){
    Errno->SetError(CGERR_ERROR, "Could not load plugin at " + Path );
    return false;
  }

  // record the handle
  Handles.push_back(handle);

  // get the function pointers
  CGPluginFunc Funcs;
  Funcs.destroy = NULL;
  Funcs.create = NULL;

  Funcs.create = (create_t*)(dlsym(handle,"__create"));
  if( !Funcs.create ){
    Errno->SetError(CGERR_ERROR,"Could not load create_t function pointer" );
    Handles.pop_back();
    dlclose(handle);
    return false;
  }

  Funcs.destroy = (destroy_t*)(dlsym(handle,"__destroy"));
  if( !Funcs.destroy ){
    Errno->SetError(CGERR_ERROR,"Could not load destroy_t function pointer" );
    Handles.pop_back();
    dlclose(handle);
    return false;
  }

  Funcs.getname = (getname_t*)(dlsym(handle,"__getname"));
  if( !Funcs.getname ){
    Errno->SetError(CGERR_ERROR,"Could not load getname_t function pointer" );
    Handles.pop_back();
    dlclose(handle);
    return false;
  }

  FuncPtrs.push_back( Funcs );

  // create the new plugin
  getname_t *GN = Funcs.getname;
  CoreGenPlugin *CGP = new CoreGenPlugin(std::string(GN()), Funcs, Env, Errno);
  Plugins.push_back(CGP);

  return true;
}
#endif

bool CoreGenPluginMgr::LoadPlugin( std::string Path ){
#if (COREGEN_PLATFORM == Darwin) || (COREGEN_PLATFORM == Linux)
  return LoadNixPlugin( Path );
#else
  return true;
#endif
}

bool CoreGenPluginMgr::ReleasePlugin( unsigned Idx ){
  if( Idx > (Plugins.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Plugin index out of bounds" );
    return false;
  }

  // execute the destructor
  //destroy_t *d = FuncPtrs[Idx].destroy;
  //d(Plugins[Idx]);
  delete(Plugins[Idx]);
  Plugins.erase(Plugins.begin()+Idx);

  // close the handle and unload the vectors
  dlclose(Handles[Idx]);
  Handles.erase(Handles.begin()+Idx);
  FuncPtrs.erase(FuncPtrs.begin()+Idx);

  return true;
}

bool CoreGenPluginMgr::ReleasePlugin( std::string Plugin ){
  for( unsigned i=0; i<Plugins.size(); i++ ){
    CoreGenPlugin *P = Plugins[i];
    if( P->GetPluginName() == Plugin ){
      return ReleasePlugin(i);
    }
  }

  return false;
}

bool CoreGenPluginMgr::ReleasePlugin( std::string Plugin,
                                      unsigned Major,
                                      unsigned Minor,
                                      unsigned Patch ){
  for( unsigned i=0; i<Plugins.size(); i++ ){
    CoreGenPlugin *P = Plugins[i];
    if( P->GetPluginName() == Plugin ){
      unsigned Ma = 0;
      unsigned Mi = 0;
      unsigned Pa = 0;
      P->GetVersion(&Ma,&Mi,&Pa);
      if( (Ma == Major) &&
          (Mi == Minor) &&
          (Pa == Patch) ){
        return ReleasePlugin(i);
      }
    }
  }

  return false;
}

CoreGenPlugin *CoreGenPluginMgr::GetPlugin(unsigned P){
  if( P > (Plugins.size()-1) ){
    Errno->SetError( CGERR_ERROR, "Plugin index out of bounds" );
    return nullptr;
  }
  return Plugins[P];
}

CoreGenPlugin *CoreGenPluginMgr::GetPlugin(std::string Plugin ){
  for( unsigned i=0; i<Plugins.size(); i++ ){
    CoreGenPlugin *P = Plugins[i];
    if( P->GetPluginName() == Plugin ){
      return P;
    }
  }
  return nullptr;
}

CoreGenPlugin *CoreGenPluginMgr::GetPlugin(std::string Plugin,
                                           unsigned Major,
                                           unsigned Minor,
                                           unsigned Patch ){
  for( unsigned i=0; i<Plugins.size(); i++ ){
    CoreGenPlugin *P = Plugins[i];
    if( P->GetPluginName() == Plugin ){
      unsigned Ma = 0;
      unsigned Mi = 0;
      unsigned Pa = 0;
      P->GetVersion(&Ma,&Mi,&Pa);
      if( (Ma == Major) &&
          (Mi == Minor) &&
          (Pa == Patch) ){
        return P;
      }
    }
  }
  return nullptr;
}

// EOF
