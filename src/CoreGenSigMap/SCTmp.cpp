//
// _SCTmp_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenSigMap/SCTmp.h"

SCTmp::SCTmp( std::string Name, unsigned Width,
              std::string Inst, std::string IRName ) :
  Name(Name), Width(Width) {
  InstMap.push_back(std::make_pair(Inst,IRName));
}

SCTmp::~SCTmp(){
}

bool SCTmp::InsertTmpPair( std::string Inst, std::string IRName ){
  InstMap.push_back(std::make_pair(Inst,IRName));
  return true;
}

std::vector<std::string> SCTmp::GetInstIRNames(std::string Inst){
  std::vector<std::string> Names;
  for( unsigned i=0; i<InstMap.size(); i++ ){
    if( InstMap[i].first == Inst )
      Names.push_back( InstMap[i].second );
  }
  return Names;
}

std::vector<std::string> SCTmp::GetIRInstNames(std::string IRName){
  std::vector<std::string> Names;
  for( unsigned i=0; i<InstMap.size(); i++ ){
    if( InstMap[i].second == IRName )
      Names.push_back( InstMap[i].first );
  }
  return Names;
}

std::pair<std::string,std::string> SCTmp::GetMap(unsigned Idx){
  if( Idx > (InstMap.size()-1) )
    return std::make_pair("","");

  return InstMap[Idx];
}

bool SCTmp::IsInstFound( std::string Inst ){
  for( unsigned i=0; i<InstMap.size(); i++ ){
    if( InstMap[i].first == Inst )
      return true;
  }
  return false;
}

bool SCTmp::IsIRFound( std::string IRName ){
  for( unsigned i=0; i<InstMap.size(); i++ ){
    if( InstMap[i].second == IRName )
      return true;
  }
  return false;
}

bool SCTmp::IsPairFound( std::string Inst, std::string IRName ){
  for( unsigned i=0; i<InstMap.size(); i++ ){
    if( (InstMap[i].first == Inst) && (InstMap[i].second == IRName) )
      return true;
  }
  return false;
}

//-- EOF
