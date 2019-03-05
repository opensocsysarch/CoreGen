//
// _CoreGenInstFormat_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"

CoreGenInstFormat::CoreGenInstFormat(std::string N,
                                     CoreGenISA *I,
                                     CoreGenErrno *E)
  : CoreGenNode(CGInstF,N,E), ISA(I) {
    InsertChild(static_cast<CoreGenNode *>(I));
}

bool CoreGenInstFormat::InsertField( std::string Name, unsigned StartBit,
                                     unsigned EndBit, CGInstField Type,
                                     bool Mand ){
  if( StartBit > EndBit ){
    return false;
  }
  Format.push_back(
    std::tuple< std::string,unsigned,
                unsigned,CGInstField,bool>(Name,StartBit,
                                           EndBit,Type,Mand) );
  // sort the vector of fields by the start bit
  std::sort(begin(Format), end(Format),
       [](std::tuple< std::string,unsigned,
                      unsigned,CoreGenInstFormat::CGInstField,bool> const &t1,
          std::tuple< std::string,unsigned,
                      unsigned,CoreGenInstFormat::CGInstField,bool> const &t2){
          return std::get<1>(t1) < std::get<1>(t2);
        }
  );
  return true;
}

bool CoreGenInstFormat::IsValidField(std::string Field ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;
  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Field == std::get<0>(*it) ){
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::InsertRegFieldMap( std::string Name,
                                           CoreGenRegClass *RClass ){
  if( !RClass ){
    return false;
  }

  // walk the vector of fields and find our target
  // target must be a register field
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( (Name == std::get<0>(*it)) &&
        (std::get<3>(*it) == CGInstReg) ){
      // found a match
      RegMap.insert( std::pair<std::string,CoreGenRegClass *>(Name,RClass) );
      CoreGenNode *N = static_cast<CoreGenNode *>(RClass);
      return InsertChild(N);
    }
  }

  return false;
}

bool CoreGenInstFormat::SetMandatoryFlag( std::string Name, bool Flag ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      std::get<4>(*it) = Flag;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::SetStartBit( std::string Name, unsigned Start ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      std::get<1>(*it) = Start;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::SetEndBit( std::string Name, unsigned End ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      std::get<2>(*it) = End;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::SetFieldType( std::string Name, CGInstField Type ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      std::get<3>(*it) = Type;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::GetMandatoryFlag( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      return std::get<4>(*it);
    }
  }
  return 0;
}

unsigned CoreGenInstFormat::GetNumMandatory(){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  unsigned M = 0;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( std::get<4>(*it)){
      M++;
    }
  }
  return M;
}

unsigned CoreGenInstFormat::GetFieldWidth( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      return (std::get<2>(*it) - std::get<1>(*it))+1;
    }
  }
  return 0;
}

unsigned CoreGenInstFormat::GetStartBit( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      return std::get<1>(*it);
    }
  }
  return 0;
}

unsigned CoreGenInstFormat::GetEndBit( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      return std::get<2>(*it);
    }
  }
  return 0;
}

std::string CoreGenInstFormat::GetFieldName( unsigned N ){
  if( N > (Format.size()-1) ){
    return " ";
  }
  return std::get<0>(Format[N]);
}

CoreGenInstFormat::CGInstField CoreGenInstFormat::GetFieldType( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<0>(*it) ){
      return std::get<3>(*it);
    }
  }
  return CGInstField::CGInstUnk;
}

CoreGenRegClass *CoreGenInstFormat::GetFieldRegClass( std::string Name ){
  std::map<std::string,CoreGenRegClass *>::iterator it;

  it = RegMap.find(Name);
  if( it != RegMap.end() ){
    return it->second;
  }
  return NULL;
}

unsigned CoreGenInstFormat::GetFormatWidth(){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool>>::iterator it;
  unsigned max = 0;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( std::get<2>(*it) > max ){
      max = std::get<2>(*it);
    }
  }

  return (max+1);
}

std::string CoreGenInstFormat::CGInstFieldToStr(CGInstField F){
  switch( F ){
  case CGInstReg:
    return "CGInstReg";
  case CGInstCode:
    return "CGInstCode";
  case CGInstImm:
    return "CGInstImm";
    break;
  case CGInstUnk:
  default:
    return "CGInstUnk";
    break;
  }
}

// EOF
