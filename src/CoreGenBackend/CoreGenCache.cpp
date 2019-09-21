//
// _CoreGenCache_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenCache.h"

CoreGenCache::CoreGenCache(std::string N, CoreGenErrno *E )
  : CoreGenNode(CGCache,E), Sets(0), Ways(0), SubLevel(false),
    ParentLevel(false), Child(nullptr){
}

CoreGenCache::CoreGenCache(std::string N, unsigned CSets,
                            unsigned CWays, CoreGenErrno *E)
  : CoreGenNode(CGCache,N,E), Sets(CSets), Ways(CWays),
    SubLevel(false), ParentLevel(false), Child(nullptr){
}

CoreGenCache::~CoreGenCache(){
}

bool CoreGenCache::SetChildCache( CoreGenCache *C ){
  if( Child ){
    return false;
  }
  Child = C;
  ParentLevel = true;
  return InsertChild(static_cast<CoreGenNode *>(Child));
}

bool CoreGenCache::SetNullChildCache(){
  Child = nullptr;
  ParentLevel = false;
  return true;
}

bool CoreGenCache::SetParentCache( CoreGenCache *P ){
  if( !P ){
    Errno->SetError(CGERR_ERROR, "Parent cache cannot be null" );
    return false;
  }

  // do not insert duplicates
  auto it = std::find(Parent.begin(),Parent.end(),P);
  if( it == Parent.end() ){
    Parent.push_back(P);
  }

  SubLevel = true;
  return true;
}

bool CoreGenCache::SetNullParentCache(){
  Parent.clear();
  SubLevel = false;
  return true;
}

CoreGenCache *CoreGenCache::GetParentCache(unsigned Idx){
  if( Idx > (Parent.size()-1) ){
    Errno->SetError(CGERR_ERROR, "Parent cache index out of bounds" );
    return nullptr;
  }

  return Parent[Idx];
}

void CoreGenCache::DeleteParentCache( CoreGenCache *C ){
  if( C == nullptr ){
    Errno->SetError( CGERR_ERROR, "DeleteEndpoint: Node pointer is null" );
  }

  // make sure the node is a parent, then delete it from
  // the parent vector
  std::vector<CoreGenCache *>::iterator it;
  it = find(Parent.begin(), Parent.end(), C);
  if( it == Parent.end() ){
    Errno->SetError( CGERR_ERROR,
                     "DeleteParentCache: Cannot find target cache in parent cache list" );
  }
  Parent.erase(it);
}

bool CoreGenCache::HasParentCache( CoreGenCache *C ){
  for( unsigned i=0; i<Parent.size(); i++ ){
    if( Parent[i] == C ){
      return true;
    }
  }
  return false;
}

CoreGenCache *CoreGenCache::GetSubCache(){
  return Child;
}

// EOF
