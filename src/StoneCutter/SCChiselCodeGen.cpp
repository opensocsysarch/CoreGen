//
// _SCChiselCodeGen_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCChiselCodeGen.h"

SCChiselCodeGen::SCChiselCodeGen( SCParser *P,
                                  SCOpts *O,
                                  SCMsg *M,
                                  std::string COF )
  : Parser(P), Opts(O), Msgs(M), ChiselFile(COF) {
  InitIntrinsics();
  InitPasses();
}

SCChiselCodeGen::~SCChiselCodeGen(){
  Intrins.clear();
  Passes.clear();
}

void SCChiselCodeGen::InitIntrinsics(){
  Intrins.push_back(static_cast<SCIntrin *>(new SCMax()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCMin()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCLoad()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCStore()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCLoadElem()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCStoreElem()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCNot()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCReverse()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCPopcount()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCClz()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCCtz()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCSext()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCZext()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCRotateL()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCRotateR()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCMaj()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCDoz()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCCompress()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCCompressM()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCInsertS()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCInsertZ()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCExtractS()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCExtractZ()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCMerge()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCConcat()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCLss()));
}

void SCChiselCodeGen::InitPasses(){
  Passes.push_back(static_cast<SCPass *>(new SCInstArg(SCParser::TheModule.get(),
                                                       Opts,
                                                       Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCInstFormat(SCParser::TheModule.get(),
                                                       Opts,
                                                       Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCPipeBuilder(SCParser::TheModule.get(),
                                                       Opts,
                                                       Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCIOWarn(SCParser::TheModule.get(),
                                                      Opts,
                                                      Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCFieldIO(SCParser::TheModule.get(),
                                                       Opts,
                                                       Msgs)));
}

void SCChiselCodeGen::WriteChiselHeader(){
  OutFile << "\\\\" << std::endl;
  OutFile << "\\\\ " << ChiselFile << std::endl;
  OutFile << "\\\\" << std::endl;
  OutFile << "\\\\ Chisel generated from StoneCutter input source" << std::endl;
  OutFile << "\\\\ " << SCCurrentDateTime();
  OutFile << "\\\\" << std::endl << std::endl;
}

bool SCChiselCodeGen::ExecutePasses(){
  bool rtn = true;

  std::vector<SCPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    SCPass *P = (*it);
    if( !P->Execute() ){
      rtn = false;
    }
  }

  return rtn;
}

bool SCChiselCodeGen::ExecuteCodegen(){

  // Execute all the necessary passes
  if( !ExecutePasses() ){
    return false;
  }

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
    Msgs->PrintMsg( L_ERROR, "Failed to open Chisel output file" );
    return false;
  }

  // write the chisel header
  WriteChiselHeader();

  // Execute the codegen
  if( !ExecuteCodegen() ){
    Msgs->PrintMsg( L_ERROR, "Failed to generate Chisel output" );
    OutFile.close();
    return false;
  }

  // close the output file
  OutFile.close();

  return true;
}

// EOF
