//
// _PluginCG_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/NodeCodegen/PluginCG.h"

PluginCG::PluginCG( CoreGenNode *N, CoreGenProj *P,
                std::string Pack, std::string Path,
                bool Comm, CoreGenErrno *E )
  : CoreGenNodeCodegen(N,P,Pack,Path,Comm,E) {
}

PluginCG::~PluginCG(){
}

bool PluginCG::Execute(CoreGenNode *Top, CoreGenNode *Parent){

  CoreGenPlugin *PluginNode = static_cast<CoreGenPlugin *>(Node->GetPlugin());

  if( !PluginNode->InitImpl(Path,CGRemoveDot(PluginNode->GetName())) )
    return false;

  if( PluginNode->HasHDLCodegen() )
    return PluginNode->ExecuteHDLCodegen(Top,Parent);

  return false;
}

// EOF
