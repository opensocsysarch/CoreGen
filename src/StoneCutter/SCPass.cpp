//
// _SCPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCPass.h"

SCPass::SCPass(std::string N,
               Module *TM,
               SCOpts *O,
               SCMsg *M)
  : Name(N), Msgs(M), Opts(O), TheModule(TM) {
  if( Opts->IsVerbose() )
    Msgs->PrintRawMsg( "Executing Pass: " + this->GetName() );
}

SCPass::~SCPass(){
}

void SCPass::PrintMsg( MSG_LEVEL L, const std::string M ){
  Msgs->PrintMsg( L, this->GetName() + " : " + M );
}

void SCPass::PrintRawMsg( const std::string M ){
  Msgs->PrintRawMsg( this->GetName() + " : " + M );
}

bool SCPass::IsGlobal( std::string Var ){
  for( auto &Global : TheModule->getGlobalList() ){
    if( Global.getName().str() == Var ){
      return true;
    }
  }
  return false;
}

bool SCPass::IsLocal( std::string Var ){
  for( auto &Global : TheModule->getGlobalList() ){
    if( Global.getName().str() == Var ){
      return false;
    }
  }
  return true;
}

bool SCPass::HasGlobalAttribute( std::string Var, std::string Attribute ){
  for( auto &Global : TheModule->getGlobalList() ){
    if( Global.getName().str() == Var ){
      AttributeSet AttrSet = Global.getAttributes();
      if( AttrSet.hasAttribute(Attribute) )
        return true;
    }
  }
  return false;
}

std::string SCPass::GetGlobalAttribute( std::string Var,
                                        std::string Attribute ){
  for( auto &Global : TheModule->getGlobalList() ){
    if( Global.getName().str() == Var ){
      AttributeSet AttrSet = Global.getAttributes();
      if( AttrSet.hasAttribute(Attribute) )
        return AttrSet.getAttribute(Attribute).getValueAsString().str();
    }
  }
  std::string Attr = "";
  return Attr;
}


// EOF
