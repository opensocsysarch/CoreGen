//
// _SCPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

std::vector<std::string> SCPass::GetInstFormats(){
  std::vector<std::string> IF;
  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("fieldtype") ){
      unsigned Idx = 0;
      while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
        IF.push_back(
          AttrSet.getAttribute("instformat"+std::to_string(Idx)).getValueAsString().str() );
        Idx++;
      }
    }
  }
  std::sort(IF.begin(),IF.end());
  IF.erase( std::unique(IF.begin(),IF.end()), IF.end() );
  return IF;
}

std::vector<std::string> SCPass::GetInstFields(std::string InstFormat){
  std::vector<std::string> Fields;

  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("fieldtype") ){
      unsigned Idx = 0;
      while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
        if( AttrSet.getAttribute("instformat"+std::to_string(Idx)).getValueAsString().str() ==
            InstFormat )
          Fields.push_back( Global.getName().str() );
        Idx++;
      }
    }
  }
  std::sort( Fields.begin(),Fields.end());
  Fields.erase( std::unique(Fields.begin(),Fields.end()), Fields.end() );
  return Fields;
}

std::vector<std::string> SCPass::GetRegClassInstFields(std::string InstFormat){
  std::vector<std::string> Fields;

  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("fieldtype") ){
      if( AttrSet.getAttribute("fieldtype").getValueAsString().str() == "register" ){
        unsigned Idx = 0;
        while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
          if( AttrSet.getAttribute("Instformat"+std::to_string(Idx)).getValueAsString().str() ==
              InstFormat )
            Fields.push_back( Global.getName().str() );
          Idx++;
        }
      }
    }
  }
  std::sort( Fields.begin(),Fields.end());
  Fields.erase( std::unique(Fields.begin(),Fields.end()), Fields.end() );
  return Fields;
}

std::vector<std::string> SCPass::GetEncodingInstFields(std::string InstFormat){
  std::vector<std::string> Fields;

  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("fieldtype") ){
      if( AttrSet.getAttribute("fieldtype").getValueAsString().str() == "encoding" ){
        unsigned Idx = 0;
        while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
          if( AttrSet.getAttribute("Instformat"+std::to_string(Idx)).getValueAsString().str() ==
              InstFormat )
            Fields.push_back( Global.getName().str() );
          Idx++;
        }
      }
    }
  }
  std::sort( Fields.begin(),Fields.end());
  Fields.erase( std::unique(Fields.begin(),Fields.end()), Fields.end() );
  return Fields;
}

std::vector<std::string> SCPass::GetImmInstFields(std::string InstFormat){
  std::vector<std::string> Fields;

  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("fieldtype") ){
      if( AttrSet.getAttribute("fieldtype").getValueAsString().str() == "immediate" ){
        unsigned Idx = 0;
        while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
          if( AttrSet.getAttribute("Instformat"+std::to_string(Idx)).getValueAsString().str() ==
              InstFormat )
            Fields.push_back( Global.getName().str() );
          Idx++;
        }
      }
    }
  }
  std::sort( Fields.begin(),Fields.end());
  Fields.erase( std::unique(Fields.begin(),Fields.end()), Fields.end() );
  return Fields;
}

std::vector<std::string> SCPass::GetRegClassInstTypes(std::string InstFormat){
  std::vector<std::string> Fields;

  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("fieldtype") ){
      if( AttrSet.getAttribute("fieldtype").getValueAsString().str() == "register" ){
        unsigned Idx = 0;
        while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
          if( AttrSet.getAttribute("Instformat"+std::to_string(Idx)).getValueAsString().str() ==
              InstFormat )
            Fields.push_back(
              AttrSet.getAttribute("regclasscontainer"+std::to_string(Idx)).getValueAsString().str() );
          Idx++;
        }
      }
    }
  }
  std::sort( Fields.begin(),Fields.end());
  Fields.erase( std::unique(Fields.begin(),Fields.end()), Fields.end() );
  return Fields;
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

unsigned SCPass::GetNumInstFormats( std::string Var ){
  for( auto &Global : TheModule->getGlobalList() ){
    if( Global.getName().str() == Var ){
      AttributeSet AttrSet = Global.getAttributes();
      if( AttrSet.hasAttribute("fieldtype") ){
        // this is a global with the same name and exists as
        // as a field in an instruction format
        unsigned NumIF = 0;
        while( AttrSet.hasAttribute("instformat"+std::to_string(NumIF)) ){
          NumIF = NumIF + 1;
        }
        return NumIF;
      }
    }
  }

  return 0;
}

unsigned SCPass::GetNumRegClasses( std::string Var ){
  for( auto &Global : TheModule->getGlobalList() ){
    if( Global.getName().str() == Var ){
      AttributeSet AttrSet = Global.getAttributes();
      if( AttrSet.hasAttribute("fieldtype") ){
        if( AttrSet.getAttribute("fieldtype").getValueAsString().str() == "register" ){
          // this is a global with the correct variable name and
          // is a register field
          unsigned NumIF = 0;
          while( AttrSet.hasAttribute("regclasscontainer"+std::to_string(NumIF)) ){
            NumIF = NumIF + 1;
          }
          return NumIF;
        }
      }
    }// end if Global.getName
  }// end for

  return 0;
}

std::string SCPass::TraceOperand( Function &F, Use *U, bool &isPredef ){
  std::string OpName;
  for( auto User : U->get()->users() ){
    if( auto Inst = dyn_cast<Instruction>(User) ){
    }
  }
  return OpName;
}

// EOF
