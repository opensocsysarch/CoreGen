//
// _CoreGenNode_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//


#include "CoreGen/CoreGenBackend/CoreGenNode.h"

CoreGenNode::CoreGenNode() : RTLType(RTLUnk), PluginNode(NULL), Errno(NULL) {}

CoreGenNode::CoreGenNode(CGNodeType T, CoreGenErrno *E)
  : Type(T), RTLType(RTLUnk), PluginNode(NULL), Errno(E) {}

CoreGenNode::CoreGenNode(CGNodeType T, std::string N, CoreGenErrno *E)
    : Name(N), Type(T), RTLType(RTLUnk), PluginNode(NULL), Errno(E) {}

CoreGenNode::~CoreGenNode() {}

CGNodeType CoreGenNode::GetType() { return Type; }

bool CoreGenNode::SetType(CGNodeType T) { Type = T; return true; }

bool CoreGenNode::SetRTLType(CGRTLType T) { RTLType = T; return true; }

bool CoreGenNode::SetErrno(CoreGenErrno *E) { Errno = E; return true; }

std::string CoreGenNode::GetName() { return Name; }

bool CoreGenNode::SetName(std::string N) { Name = N; return true; }

unsigned CoreGenNode::GetNumChild() { return CNodes.size(); }

bool CoreGenNode::IsOverridden() {
  if( PluginNode ){
    return true;
  }
  return false;
}

CoreGenNode *CoreGenNode::GetOverriddenNode(){
  return PluginNode;
}

bool CoreGenNode::SetOverriddenNode( CoreGenNode *Node ){
  PluginNode = Node;
  return true;
}

bool CoreGenNode::RemoveOverriddenNode(){
  PluginNode = nullptr;
  return true;
}

CoreGenNode *CoreGenNode::GetChild(unsigned I){
    if( I>(CNodes.size()-1) ){
      return NULL;
    }
    return CNodes[I];
  }

bool CoreGenNode::IsDuplicateNode( CoreGenNode *N ){
  if( !N ){ return false; }

  for( unsigned i=0; i<CNodes.size(); i++ ){
      if( (CNodes[i]->GetName() == N->GetName()) &&
          (CNodes[i]->GetType() == N->GetType())){
        // duplicate found
        return true;
      }
  }
  return false;
}

bool CoreGenNode::InsertChild( CoreGenNode *N ){
    if( !N ){ return false; }
    // search for duplicates
    if( IsDuplicateNode(N) ){
      Errno->SetError(CGERR_WARN, "Duplicate child node found: " +
                      N->GetName() );
      return true;
    }
    CNodes.push_back(N);
    return true;
}

bool CoreGenNode::InsertPlugin( CoreGenNode *N ){
  return InsertChild(N);
}

bool CoreGenNode::DeleteChild( CoreGenNode *N ){
  if( !N ){ return false; }
    for( unsigned i=0; i<CNodes.size(); i++ ){
      if( CNodes[i] == N ){
        CNodes.erase(CNodes.begin()+i);
      }
    }
  return true;
}

bool CoreGenNode::SetRTL( std::string R ){
  if( R.length() > 0 ){
    RTL = R;
    return true;
  }
  return false;
}

bool CoreGenNode::IsRTL() {
  if( RTL.length() > 0 ){
    return true;
  }
  return false;
}

std::string CoreGenNode::GetRTL(){
  return RTL;
}

CGRTLType CoreGenNode::GetRTLType(){
  return RTLType;
}

bool CoreGenNode::SetRTLFile( std::string RTLF ){
  if( RTLF.length() > 0 ){
    RTLFile = RTLF;
    return true;
  }
  return false;
}

bool CoreGenNode::IsRTLFile(){
  if( RTLFile.length() > 0 ){
    return true;
  }
  return false;
}

std::string CoreGenNode::GetRTLFile(){
  return RTLFile;
}

bool CoreGenNode::SetAttr( CGAttr A ){
  Attrs.push_back( A );
  std::sort( Attrs.begin(), Attrs.end() );
  Attrs.erase( std::unique( Attrs.begin(), Attrs.end() ), Attrs.end() );
  return true;
}

bool CoreGenNode::HasAttr(CGAttr A){
  for( unsigned i=0; i<Attrs.size(); i++ ){
    if( Attrs[i] == A ){
      return true;
    }
  }
  return false;
}

// EOF
