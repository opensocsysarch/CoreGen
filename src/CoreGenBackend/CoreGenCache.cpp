//
// _CoreGenCache_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenCache.h"

CoreGenCache::CoreGenCache(std::string N, CoreGenErrno *E )
  : CoreGenNode(CGCache,E), Sets(0), Ways(0), SubLevel(false),
    ParentLevel(false), Child(nullptr), Parent(nullptr){
}

CoreGenCache::CoreGenCache(std::string N, unsigned CSets,
                            unsigned CWays, CoreGenErrno *E)
  : CoreGenNode(CGCache,N,E), Sets(CSets), Ways(CWays),
    SubLevel(false), ParentLevel(false), Child(nullptr), Parent(nullptr){
}

CoreGenCache::~CoreGenCache(){
}

bool CoreGenCache::SetChildCache( CoreGenCache *C ){
  if( Child ){
    return false;
  }
  Child = C;
  ParentLevel = true;
  InsertChild(static_cast<CoreGenNode *>(Child));
  return true;
}

bool CoreGenCache::SetParentCache( CoreGenCache *P ){
  Parent = P;
  SubLevel = true;
  return true;
}

CoreGenCache *CoreGenCache::GetParentCache(){
  return Parent;
}

CoreGenCache *CoreGenCache::GetSubCache(){
  return Child;
}

// EOF
