//
// _SCInstArg_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCInstArg.h"

SCInstArg::SCInstArg(Module *TM,
                     SCOpts *O,
                     SCMsg *M)
  : SCPass("InstArg",TM,O,M) {
}

SCInstArg::~SCInstArg(){
  RegFileVect.clear();
}

bool SCInstArg::GetRegClassMap(){
  std::map<std::string,std::string> SubRegMap;

  // retrieve a list of the global variables
  for(auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    // check for the proper attributes
    if( AttrSet.hasAttribute("fieldtype") ){
      // record all the instruction format fields
      RegFileVect.push_back(Global.getName().str());
    }else if( AttrSet.hasAttribute("regfile") ){
      // this is a register file definition
      RegFileVect.push_back(
        AttrSet.getAttribute("regfile").getValueAsString().str());
    }else if( !AttrSet.hasAttribute("subregister") ){
      // normal register
      if( !AttrSet.hasAttribute("regclass") ){
        this->PrintMsg( L_ERROR,
                      "Global variable definition has no register class attribute: "
                      + Global.getName().str());
        return false;
      }else if( !AttrSet.hasAttribute("register") ){
        this->PrintMsg( L_ERROR,
                      "Global variable definition has no register attribute: "
                      + Global.getName().str());
        return false;
      }
      // insert into register class map as "register : register_class"
      RegClassMap.insert(
        std::pair<std::string,std::string>(
          AttrSet.getAttribute("register").getValueAsString().str(),
          AttrSet.getAttribute("regclass").getValueAsString().str() ) );
    }else{
      // subregister
      if( !AttrSet.hasAttribute("register") ){
        this->PrintMsg( L_ERROR,
                      "Global variable definition has no register attribute: "
                      + Global.getName().str());
        return false;
      }

      // insert into temporary map
      SubRegMap.insert(
        std::pair<std::string,std::string>(
          AttrSet.getAttribute("subregister").getValueAsString().str(),
          AttrSet.getAttribute("register").getValueAsString().str() ) );
    }
  }

  // fix up the RegClassMap with any subregister references
  std::map<std::string,std::string>::iterator it;
  for( it=SubRegMap.begin(); it!= SubRegMap.end(); ++it ){
    // retrieve the register class for the target subregister
    std::map<std::string,std::string>::iterator pos =
      RegClassMap.find(it->second);
    if( pos == RegClassMap.end() ){
      this->PrintMsg( L_ERROR,
                      "Global subregister variable has no register class attribute: "
                      + it->first );
      return false;
    }
    RegClassMap.insert(
      std::pair<std::string,std::string>(it->first,pos->first) );
  }

  // print them
#if 0
  for( it=RegClassMap.begin(); it != RegClassMap.end(); ++it ){
    std::cout << it->first << " : " << it->second << std::endl;
  }
#endif
  return true;
}

bool SCInstArg::CheckInstArgs(){

  bool rtn = true;

  // walk the set of functions and grab their argument lists
  for(auto &Func : TheModule->getFunctionList() ){

    // check for lazy arguments
    if( Func.hasLazyArguments() ){
      this->PrintMsg( L_ERROR,
                      "Instruction definitions are not permitted to have lazy arguments: "
                      + Func.getName().str());
      rtn = false;
    }else if( Func.isVarArg() ){
      this->PrintMsg( L_ERROR,
                      "Instruction definitions are not permitted to have variadic arguments: "
                      + Func.getName().str());
      rtn = false;
    }

    // we only need to examine local functions
    // our stonecutter intrinsics are currently defined as function decl's
    // with no bodies
    if( Func.begin() != Func.end() ){
      // walk all the arguments
      for( auto FuncArg = Func.arg_begin(); FuncArg != Func.arg_end(); ++FuncArg ){
        // registers and subregisters
        std::map<std::string,std::string>::iterator pos =
          RegClassMap.find(FuncArg->getName().str());
        // register files
        std::vector<std::string>::iterator it =
          find(RegFileVect.begin(), RegFileVect.end(), FuncArg->getName().str());
        if( (pos == RegClassMap.end()) && (it == RegFileVect.end()) ){
          this->PrintMsg( L_ERROR,
                        "Instruction definition contains an argument with no backing register or register file : Instruction="
                        + Func.getName().str() + "; Argument=" + FuncArg->getName().str() );
          rtn = false;
        }
      }
    }
  }

  return rtn;
}

bool SCInstArg::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // retrieve the register class attribute map
  if( !GetRegClassMap() ){
    return false;
  }

  // determine if each instruction's argument list is valid
  if( !CheckInstArgs() ){
    return false;
  }

  return true;
}

// EOF
