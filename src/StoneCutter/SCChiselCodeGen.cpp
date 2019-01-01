//
// _SCChiselCodeGen_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCChiselCodeGen.h"

SCChiselCodeGen::SCChiselCodeGen( SCParser *P, SCMsg *M,
                          std::string COF )
  : Parser(P), Msgs(M), ChiselFile(COF) {
}

SCChiselCodeGen::~SCChiselCodeGen(){
}

void SCChiselCodeGen::WriteChiselHeader(){
  OutFile << "\\\\" << std::endl;
  OutFile << "\\\\ " << ChiselFile << std::endl;
  OutFile << "\\\\" << std::endl;
  OutFile << "\\\\ Chisel generated from StoneCutter input source" << std::endl;
  OutFile << "\\\\ " << SCCurrentDateTime();
  OutFile << "\\\\" << std::endl << std::endl;
}

bool SCChiselCodeGen::ExecuteCodegen(){
  // walk all the functions in the module
  for( auto curFref = SCParser::TheModule->begin(),
            endFref = SCParser::TheModule->end();
            curFref != endFref; ++curFref ){
    //std::cout << "Found function: " << curFref->getName().str() << std::endl;
    // walk all the basic blocks
    for( auto curBB = curFref->begin(),
              endBB = curFref->end();
              curBB != endBB; ++curBB ){
      // walk all the instructions
      for( auto curInst = curBB->begin(),
                endInst = curBB->end();
                curInst != endInst; ++curInst ){
        //std::cout << "\t\tFound instruction: " << curInst->getOpcodeName() << std::endl;
      }
    }
  }

  return true;
}

bool SCChiselCodeGen::GenerateChisel(){

  if( !Parser ){
    return false;
  }

  // this should probably be deprecated
  if( ChiselFile.length() == 0 ){
    Msgs->PrintMsg( L_ERROR, "Chisel output file cannot be null" );
    return false;
  }

  // open the output file
  OutFile.open(ChiselFile, std::ios::trunc);
  if( !OutFile.is_open() ){
    Msgs->PrintMsg( L_ERROR, "Failed to open chisel output file" );
    return false;
  }

  // write the chisel header
  WriteChiselHeader();

  // Execute the codegen
  if( !ExecuteCodegen() ){
    Msgs->PrintMsg( L_ERROR, "Failed to generate chisel output" );
    OutFile.close();
    return false;
  }

  // close the output file
  OutFile.close();

  return true;
}

// EOF
