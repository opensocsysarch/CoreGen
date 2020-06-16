//
// _CoreGenInstFormat_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
  if( ISA )
    InsertChild(static_cast<CoreGenNode *>(ISA));
}

bool CoreGenInstFormat::SetISA(CoreGenISA *I){
  if( !I )
    return false;

  if( ISA ){
    DeleteChild(ISA);
  }
  ISA = I;
  return InsertChild(I);
}

bool CoreGenInstFormat::InsertField( std::string Name, unsigned StartBit,
                                     unsigned EndBit, CGInstField Type,
                                     bool Mand ){
  if( StartBit > EndBit ){
    return false;
  }

  // ensure that this new field doesn't conflict with adjacent fields
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;
  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      Errno->SetError(CGERR_ERROR,
                      "Attempting to insert a duplicate field; Name="+Name+"; InstFormat="+this->GetName());
      return false;
    }
  }

  bool reg_is_dest = false;
  Format.push_back(
    std::tuple< std::string,unsigned,
                unsigned,CGInstField,bool,bool>(Name,StartBit,
                                           EndBit,Type,Mand, reg_is_dest) );
  // sort the vector of fields by the start bit
  std::sort(begin(Format), end(Format),
       [](std::tuple< std::string,unsigned,
                      unsigned,CoreGenInstFormat::CGInstField,bool, bool> const &t1,
          std::tuple< std::string,unsigned,
                      unsigned,CoreGenInstFormat::CGInstField,bool, bool> const &t2){
          return std::get<CGFormatStartBit>(t1) < std::get<CGFormatStartBit>(t2);
        }
  );
  return true;
}

bool CoreGenInstFormat::IsValidField(std::string Field ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;
  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Field == std::get<CGFormatName>(*it) ){
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
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( (Name == std::get<CGFormatName>(*it)) &&
        (std::get<CGFormatType>(*it) == CGInstReg) ){
      // found a match
      RegMap.insert( std::pair<std::string,CoreGenRegClass *>(Name,RClass) );
      CoreGenNode *N = static_cast<CoreGenNode *>(RClass);
      if( !IsDuplicateNode(N) )
        return InsertChild(N);
      else
        return true;
    }
  }

  return false;
}

bool CoreGenInstFormat::SetMandatoryFlag( std::string Name, bool Flag ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      std::get<CGFormatMandatory>(*it) = Flag;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::SetRegFieldIsDestFlag( std::string Name, bool Flag ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      std::get<CGFormatRegIsDest>(*it) = Flag;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::SetStartBit( std::string Name, unsigned Start ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      std::get<CGFormatStartBit>(*it) = Start;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::SetEndBit( std::string Name, unsigned End ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      std::get<CGFormatEndBit>(*it) = End;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::SetFieldType( std::string Name, CGInstField Type ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      std::get<CGFormatType>(*it) = Type;
      return true;
    }
  }
  return false;
}

bool CoreGenInstFormat::GetMandatoryFlag( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      return std::get<CGFormatMandatory>(*it);
    }
  }
  return 0;
}

unsigned CoreGenInstFormat::GetNumMandatory(){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  unsigned M = 0;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( std::get<CGFormatMandatory>(*it)){
      M++;
    }
  }
  return M;
}

unsigned CoreGenInstFormat::GetFieldWidth( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      return (std::get<CGFormatEndBit>(*it) - std::get<CGFormatStartBit>(*it))+1;
    }
  }
  return 0;
}

unsigned CoreGenInstFormat::GetStartBit( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      return std::get<CGFormatStartBit>(*it);
    }
  }
  return 0;
}

unsigned CoreGenInstFormat::GetEndBit( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      return std::get<CGFormatEndBit>(*it);
    }
  }
  return 0;
}

std::string CoreGenInstFormat::GetFieldName( unsigned N ){
  if( N > (Format.size()-1) ){
    return " ";
  }
  return std::get<CGFormatName>(Format[N]);
}

CoreGenInstFormat::CGInstField CoreGenInstFormat::GetFieldType( std::string Name ){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      return std::get<CGFormatType>(*it);
    }
  }
  return CGInstField::CGInstUnk;
}

bool CoreGenInstFormat::SetNullField( std::string Name ){
  // nullify the field
  if( !this->SetFieldType(Name,CoreGenInstFormat::CGInstUnk) )
    return false;

  // remove the entry in the regclass map
  RegMap.erase(Name);

  return true;
}

CoreGenRegClass *CoreGenInstFormat::GetFieldRegClass( std::string Name ){
  std::map<std::string,CoreGenRegClass *>::iterator it;

  it = RegMap.find(Name);
  if( it != RegMap.end() ){
    return it->second;
  }
  return nullptr;
}

unsigned CoreGenInstFormat::GetFormatWidth(){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;
  unsigned max = 0;

  if( Format.size() == 0 ){
    return 0;
  }

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( std::get<CGFormatEndBit>(*it) > max ){
      max = std::get<CGFormatEndBit>(*it);
    }
  }

  return (max+1);
}

bool CoreGenInstFormat::GetRegFieldIsDest(std::string Name){
  std::vector<std::tuple<std::string,unsigned,unsigned,CGInstField,bool,bool>>::iterator it;

  for( it=Format.begin(); it != Format.end(); ++it ){
    if( Name == std::get<CGFormatName>(*it) ){
      return std::get<CGFormatRegIsDest>(*it);
    }
  }
  return 0;
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
