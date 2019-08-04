//
// _SCPipeInfo_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCPipeInfo.h"

SCPipeInfo::SCPipeInfo(Module *TM, SCOpts *O, SCMsg *M)
  : TheModule(TM), Opts(O), Msgs(M), PCPresent(false) {
    // initialize the PC register
    PCReg.first = "SIGTEMP";
    PCReg.second = "PC";

    // Derive all the register fields and immediates
    for( auto &Global : TheModule->getGlobalList() ){
      // walk all the global variables and retrieve their attribute lists
      // look for the appropriate set of immediate and register encoding fields
      AttributeSet AttrSet = Global.getAttributes();

      if( AttrSet.hasAttribute("fieldtype") ){
        if( AttrSet.getAttribute("fieldtype").getValueAsString().str() == "immediate" ){
          // found an immediate field
          unsigned Idx = 0;
          while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
            ImmFields.push_back(
              std::make_pair<std::string,std::string>(
                AttrSet.getAttribute("instformat"+std::to_string(Idx)).getValueAsString().str(),
                Global.getName().str()) );
            Idx++;
          }
        }else if( AttrSet.getAttribute("fieldtype").getValueAsString().str() == "register" ){
          // found a register field
          unsigned Idx = 0;
          while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
            RegFields.push_back(
              std::make_pair<std::string,std::string>(
                AttrSet.getAttribute("instformat"+std::to_string(Idx)).getValueAsString().str(),
                Global.getName().str()) );
            Idx++;
          }
        }
        // else, ignore the encoding fields
      }else if( AttrSet.hasAttribute("register") &&
                AttrSet.hasAttribute("pc") ){
        if( AttrSet.getAttribute("pc").getValueAsString().str() == "true" ){
          // found a pc register
          PCReg.first  = AttrSet.getAttribute("regclass").getValueAsString().str();
          PCReg.second = AttrSet.getAttribute("register").getValueAsString().str();
          PCPresent = true;
        }
      }
    }

    // Derive the unique set of register fields and immediates
    for( auto it = ImmFields.begin(); it != ImmFields.end(); ++it ){
      UniqueImmFields.push_back( it->second );
    }
    for( auto it = RegFields.begin(); it != RegFields.end(); ++it ){
      UniqueRegFields.push_back( it->second );
    }
    std::sort( UniqueImmFields.begin(), UniqueImmFields.end() );
    UniqueImmFields.erase( std::unique( UniqueImmFields.begin(), UniqueImmFields.end() ),
                           UniqueImmFields.end() );
    std::sort( UniqueRegFields.begin(), UniqueRegFields.end() );
    UniqueRegFields.erase( std::unique( UniqueRegFields.begin(), UniqueRegFields.end() ),
                           UniqueRegFields.end() );
}

SCPipeInfo::~SCPipeInfo(){
}

std::string SCPipeInfo::GetRegFieldInstFormat( unsigned Idx ){
  if( Idx > (RegFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Reg field index out of range" );
    return "";
  }

  return RegFields[Idx].first;
}

std::string SCPipeInfo::GetRegFieldName( unsigned Idx ){
  if( Idx > (RegFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Reg field index out of range" );
    return "";
  }

  return RegFields[Idx].second;
}

std::string SCPipeInfo::GetUniqueFieldName( unsigned Idx ){
  if( Idx > (UniqueRegFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Unique reg field index out of range" );
    return "";
  }
  return UniqueRegFields[Idx];
}

std::string SCPipeInfo::GetImmFieldInstFormat( unsigned Idx ){
  if( Idx > (ImmFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Imm field index out of range" );
    return "";
  }
  return ImmFields[Idx].first;
}

std::string SCPipeInfo::GetImmFieldName( unsigned Idx ){
  if( Idx > (ImmFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Imm field index out of range" );
    return "";
  }
  return ImmFields[Idx].second;
}

std::string SCPipeInfo::GetUniqueImmFieldName( unsigned Idx ){
  if( Idx > (UniqueImmFields.size()-1) ){
    Msgs->PrintMsg( L_ERROR, "Unique imm field index out of range" );
    return "";
  }
  return UniqueImmFields[Idx];
}

bool SCPipeInfo::IsRegisterField( std::string Field ){
  if( Field.length() == 0 )
    return false;

  for( unsigned i=0; i<UniqueRegFields.size(); i++ ){
    if( UniqueRegFields[i] == Field )
      return true;
  }

  return false;
}

bool SCPipeInfo::IsImmField( std::string Field ){
  if( Field.length() == 0 )
    return false;

  for( unsigned i=0; i<UniqueImmFields.size(); i++ ){
    if( UniqueImmFields[i] == Field )
      return true;
  }

  return false;
}

std::string SCPipeInfo::GetPCName(){
  return PCReg.second;
}

std::string SCPipeInfo::GetPCRegClass(){
  return PCReg.first;
}

// EOF
