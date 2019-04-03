//
// _SCInstFormat_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCInstFormat.h"

SCInstFormat::SCInstFormat(Module *TM,
                           SCOpts *O,
                           SCMsg *M)
  : SCPass("InstFormat",TM,O,M) {
}

SCInstFormat::~SCInstFormat(){
  RegFileVect.clear();
}

bool SCInstFormat::CheckInstFormat(){
  // Walk all the globals and interrogate any instruction
  // field that is listed as a register class.
  // Search the register file vector and see if it exists.
  // If it doesn't, flag it and notify the user
  bool rtn = true;
  for( auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("fieldtype") ){
      if( AttrSet.getAttribute("fieldtype").getValueAsString().str() ==
          "register" ){

        // make sure it has instformats
        if( this->GetNumInstFormats(Global.getName().str()) == 0 ){
          rtn = false;
          this->PrintMsg( L_ERROR, "Instruction field=" +
                          Global.getName().str() + " has no 'instformat' attribute" );
        }

        // make sure all the instformats have register class containersa
        for( unsigned i=0; i<this->GetNumInstFormats(Global.getName().str()); i++ ){
          // test entry 'i'
          std::string InstF = this->GetGlobalAttribute(Global.getName().str(),
                                                       "instformat"+std::to_string(i));
          if( !this->HasGlobalAttribute(Global.getName().str(),
                                        "regclasscontainer"+std::to_string(i)) ){
            rtn = false;
            this->PrintMsg( L_ERROR, "Instruction field=" +
                            Global.getName().str() + " in instruction format=" +
                            InstF + " has no matching 'regclasscontainer' attribute" );
          }

          std::string Container = this->GetGlobalAttribute(Global.getName().str(),
                                                           "regclasscontainer"+std::to_string(i));

          if( std::find(RegFileVect.begin(),RegFileVect.end(),Container) ==
              RegFileVect.end() ){
            rtn = false;
            this->PrintMsg( L_ERROR, "Instruction field=" +
                            Global.getName().str() + " in instruction format=" +
                            InstF + " is not a member of a known register class; regclass=" +
                            Container );
          }
        }
      }
    }
  }
  return rtn;
}

bool SCInstFormat::GetRegFileVect(){
  // Walk all the globals and build our vector of appropriate
  // register file instances.
  for(auto &Global : TheModule->getGlobalList() ){
    AttributeSet AttrSet = Global.getAttributes();
    if( AttrSet.hasAttribute("regfile") ){
      RegFileVect.push_back(
        AttrSet.getAttribute("regfile").getValueAsString().str());
    }
  }
  return true;
}

bool SCInstFormat::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  if( !GetRegFileVect() ){
    return false;
  }

  if( !CheckInstFormat() ){
    return false;
  }

  return true;
}

// EOF
