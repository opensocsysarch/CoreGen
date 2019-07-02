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

std::string SCPass::GetGlobalRegClass( std::string Field,
                                       std::string Format ){
  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("field_name") ){
      if( AttrSet.getAttribute("field_name").getValueAsString().str() == Field ){
        unsigned Idx = 0;
        while( AttrSet.hasAttribute("instformat"+std::to_string(Idx)) ){
          if( AttrSet.getAttribute("Instformat"+std::to_string(Idx)).getValueAsString().str() ==
              Format ){
            // we found the correct index, retrieve the associated regclasscontainer
            return AttrSet.getAttribute("regclasscontainer"+std::to_string(Idx)).getValueAsString().str();
          }
          Idx++;
        }
      }
    }
  }
  return "";
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

std::string SCPass::StrToUpper(std::string S){
  for( unsigned i=0; i<S.length(); i++ ){
    S[i] = toupper(S[i]);
  }
  return S;
}

std::string SCPass::TraceOperand( Function &F, Value *V,
                                  bool &isPredef, bool &isImm,
                                  unsigned &Width ){
  // check to see if the value is a constant
  if( auto CInt = dyn_cast<ConstantInt>(V) ){
    isImm = true;
    isPredef = false;
    Width = (unsigned)(CInt->getSExtValue());
    return CInt->getName().str();
  }

  // check to see if the operand is a register
  if( HasGlobalAttribute(V->getName().str(),"register") ){
    // derive the register class type and return it
    isPredef = true;
    Width = 1;    // derive the register width
    return V->getName().str() + "_" + GetGlobalAttribute(V->getName().str(),"regclass");
  }

  // check to see if the operand is a instruction field
  if( HasGlobalAttribute(V->getName().str(),"fieldtype") ){
    if( GetGlobalAttribute(V->getName().str(),"fieldtype") == "register" ){
      // derive the register class type and return it
      isPredef = true;
      isImm = false;
      Width = 1;  // need to derive the register width
      return V->getName().str() + "_" + GetGlobalRegClass( V->getName().str(),
                                GetGlobalAttribute(F.getName().str(),
                                                   "instformat") );
    }else if( GetGlobalAttribute(V->getName().str(),"fieldtype") == "encoding" ){
      // return the field name
      isPredef = true;
      isImm = false;
      Width = 1;  // need to derive the encoding width
      return V->getName().str();
    }else if( GetGlobalAttribute(V->getName().str(),"fieldtype") == "immediate" ){
      // return the field name as this mimics an instruction payload read
      isPredef = true;
      isImm = true;
      Width = 1;  // need to derive the immediate width
      return V->getName().str();
    }
  }

  // check to see if the operand is a register class
  if( HasGlobalAttribute(V->getName().str(),"regclass") ){
    // this is a global var
    isPredef = true;
    Width = 1;
    return V->getName().str();
  }

  // The current operand was not in our global list, search for all its uses
  // and determine if it has a global root operand.  If not, return the tmp
  // name as it requires a custom operand
  for( auto User : V->users() ){
    if( auto Inst = dyn_cast<Instruction>(User) ){
      if( Inst->getOpcode() == Instruction::Store ){
        // examine the Store instructions
        // This would be a case where we have one of the following:
        //
        // store [type] %REGISTER, [type] %TMP0
        // ...or...
        // store [type] %REGISTER, [type] %TMP0
        // %TMP1 = <op> [type] %REGISTER, %REGISTER
        // store [type] %TMP1, %TMP0
        // ...or...
        // store [type] %TMP, %REGISTER
        //
        if( Inst->getOperand(0)->getName().str() == V->getName().str() ){
          // The source operand is our operand, check the target operand
          // and see if it exists as a global
          return TraceOperand(F,Inst->getOperand(1),isPredef,isImm,Width);
        }else{
          // The target operand is our operand, check the source operand
          // and see if it exists as a global
          return TraceOperand(F,Inst->getOperand(0),isPredef,isImm,Width);
        }
      }else if( (Inst->getOpcode() != Instruction::Load) && Inst->hasName() ){
        // else, examine the target of the instruction
        Value *LHS = cast<Value>(Inst);
        //std::cout << "LHS = " << LHS->getName().str() << " from " << F.getName().str() << std::endl;
        return TraceOperand(F,LHS,isPredef,isImm,Width);
      }
    }
  }
  // if we reach this point, then the source is unique
  isImm = false;
  isPredef = false;
  Width = V->getType()->getIntegerBitWidth();
  return V->getName().str();
}

// EOF
