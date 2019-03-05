//
// _SCFieldIO_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCFieldIO.h"

SCFieldIO::SCFieldIO(Module *TM,
                           SCOpts *O,
                           SCMsg *M)
  : SCPass("FieldIO",TM,O,M) {
}

SCFieldIO::~SCFieldIO(){
  Fields.clear();
}

bool SCFieldIO::RetrieveFields(){
  std::vector<std::string> Formats = this->GetInstFormats();

  for( unsigned i=0; i<Formats.size(); i++ ){
    std::vector<std::string> TmpFields = this->GetInstFields(Formats[i]);

    for( unsigned j=0; j<TmpFields.size(); j++ ){
      if( this->GetGlobalAttribute( TmpFields[j], "fieldtype" ) !=
          "register" ){
        // this is an encoding or immediate field
        Fields.push_back(TmpFields[i]);
      }
    }
  }

  // remove the duplicates
  std::sort( Fields.begin(), Fields.end() );
  Fields.erase( std::unique(Fields.begin(),Fields.end()), Fields.end() );
  return true;
}

bool SCFieldIO::CheckInstArgs( Function &F, Instruction &I ){

  bool Rtn = true;

  if( I.getOpcode() == Instruction::Store ){
    for( auto U : I.getOperand(1)->users() ){
      if( auto Inst = dyn_cast<Instruction>(U) ){
        for( unsigned i=0; i<Inst->getNumOperands(); i++ ){
          // check to see if the i'th operand is in our field list
          std::vector<std::string>::iterator it = std::find(Fields.begin(),
                                                            Fields.end(),
                                                            Inst->getOperand(i)->getName().str() );
          if( it != Fields.end() ){
            // print an error
            this->PrintMsg( L_ERROR, "Cannot write to read-only field: " + *it );
            Rtn = false;
          }
        }
      }
    }
  }

  return Rtn;
}

bool SCFieldIO::ExamineFieldIO(){
  bool Rtn = true;
  // walk all the functions
  for( auto &Func : TheModule->getFunctionList() ){
    // walk all the basic blocks
    for( auto &BB : Func.getBasicBlockList() ){
      // walk all the instructions
      for( auto &Inst : BB.getInstList() ){
        if( !CheckInstArgs( Func, Inst ) ){
          Rtn = false;
        }
      }
    }
  }
  return Rtn;
}

bool SCFieldIO::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  if( !RetrieveFields() ){
    return false;
  }

  if( !ExamineFieldIO() ){
    return false;
  }

  return true;
}

// EOF
