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
  : Parser(P), Opts(O), Msgs(M), ChiselFile(COF), SM(nullptr) {
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
  Intrins.push_back(static_cast<SCIntrin *>(new SCNeg()));
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
  Intrins.push_back(static_cast<SCIntrin *>(new SCFence()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCBsel()));
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
  OutFile << "//" << std::endl;
  OutFile << "// " << ChiselFile << std::endl;
  OutFile << "//" << std::endl;
  OutFile << "// Chisel generated from StoneCutter input source" << std::endl;
  OutFile << "// " << SCCurrentDateTime();
  OutFile << "//" << std::endl << std::endl;
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

void SCChiselCodeGen::WriteUCodeTableComment(SCPipeInfo *PInfo){
  OutFile << std::endl << std::endl;
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << "// Microcode Table Fields" << std::endl;
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << "// | LABEL | LD_IR | REG_SEL | REG_WR | EN_REG ";

  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "| LD_" << PInfo->GetUniqueFieldName(i) << " ";
  }

  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "| IMM_SEL | EN_IMM | ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_TARGET |" << std::endl;
  }else{
    OutFile << "| ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_TARGET |" << std::endl;
  }
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << "// Descriptions" << std::endl;
  OutFile << "// - LD_IR = Load instruction register" << std::endl;
  OutFile << "// - REG_SEL = Register address select" << std::endl;
  OutFile << "// - REG_WR = Determines if operation is read/write" << std::endl;
  OutFile << "// - EN_REG = Register enable signal **" << std::endl;

  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "// - LD_" << PInfo->GetUniqueFieldName(i) << " = AMS Register enable" << std::endl;
  }

  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "// - IMM_SEL = Determines the immediate field to select" << std::endl;
    OutFile << "// - EN_IMM = Immediate enable signal **" << std::endl;
  }
  OutFile << "// - ALU_OP = Determines the type of ALU operation" << std::endl;
  OutFile << "// - EN_ALU = ALU enable signal **" << std::endl;
  OutFile << "// - LD_MA = Load memory address" << std::endl;
  OutFile << "// - MEM_WR = Determines is opeation is read/write" << std::endl;
  OutFile << "// - EN_MEM = Memory enable signal **" << std::endl;
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << "// ** - Only one of these signals can be selected within a give uOp" << std::endl;
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << std::endl << std::endl;
}

bool SCChiselCodeGen::ExecuteUcodeCodegen(){
  // retrieve the package and isa names
  std::string Package = Opts->GetPackage();
  std::string ISA = Opts->GetISA();

  if( Package.length() == 0 ){
    Package = ISA;
  }

  // derive all the field info for our ISA
  SCPipeInfo *PInfo = new SCPipeInfo( SCParser::TheModule.get(),
                                      Opts,
                                      Msgs );
  if( PInfo == nullptr ){
    Msgs->PrintMsg( L_ERROR, "Could not generate pipe info from IR attributes" );
    return false;
  }

  // write the package info and required chisel packages
  OutFile << "package " << Package << std::endl;
  OutFile << "{" << std::endl;

  OutFile << "import chisel3._" << std::endl;
  OutFile << "import chisel3.util._" << std::endl;
  OutFile << "import Constants._" << std::endl;
  OutFile << "import Common._" << std::endl;

  // TODO: generate all the temporary registers inside the core

  // write out a comment block to describe all the fields
  WriteUCodeTableComment(PInfo);

  // write out the microcode object
  OutFile << "object " << ISA << "Microcode" << std::endl;
  OutFile << "{" << std::endl;
  OutFile << "\tval codes = Array[" << ISA << "MicroOp](" << std::endl;

  // Stage 1: write out the predefined operations
  // These include:
  // - FETCH : fetch new instruction
  // - NOP : no operation
  // - ILLEGAL : illegal operation
  // - UNIMPL : unimplemented instruction
  // - INIT_PC : init the PC


  // write out the closure of the table structure
  OutFile << "\t)" << std::endl;
  OutFile << "}" << std::endl;

  // write the footer
  OutFile << "}" << std::endl;

  delete PInfo;

  return true;
}

bool SCChiselCodeGen::ExecuteManualCodegen(){
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

bool SCChiselCodeGen::ExecuteCodegen(){

  // Execute all the necessary passes
  if( !Opts->IsPassRun() ){
    if( !ExecutePasses() ){
      return false;
    }
  }

  // attempt to read the signal map back out
  CoreGenSigMap *CSM = nullptr;
  if( SigMap.length() > 0 ){
    CSM = new CoreGenSigMap();
    if( !CSM->ReadSigMap( SigMap ) ){
      Msgs->PrintMsg( L_ERROR, "Could not read signal map from file: " + SigMap );
      return false;
    }
    return ExecuteUcodeCodegen();
  }else{
    return ExecuteManualCodegen();
  }
}

bool SCChiselCodeGen::ExecuteSignalMap(){
  // create a new signal map object
  SCSigMap *SM = new SCSigMap(SCParser::TheModule.get(),
                              Opts,
                              Msgs);
  if( SM == nullptr )
    return false;

  // set the signal map file
  if( !SM->SetSignalMapFile(SigMap) ){
    delete SM;
    return false;
  }

  // set the intrinsics vector
  if( !SM->SetIntrins(&Intrins) ){
    delete SM;
    return false;
  }

  // execute the signal map pass
  bool rtn = true;
  if( !SM->Execute() )
    rtn = false;

  // delete the signal map object
  delete SM;

  return rtn;
}

bool SCChiselCodeGen::GenerateSignalMap(std::string SM){
  if( SM.length() == 0 ){
    Msgs->PrintMsg( L_ERROR, "Signal map output full is null" );
    return false;
  }
  SigMap = SM;

  if( !Parser ){
    Msgs->PrintMsg( L_ERROR, "No parsing input for generating signal map" );
    return false;
  }

  if( !ExecutePasses() ){
    Msgs->PrintMsg( L_ERROR, "Failed to execute passes for signal map" );
    return false;
  }

  Opts->PassRun();

  if( !ExecuteSignalMap() ){
    Msgs->PrintMsg( L_ERROR, "Failed to generate signal map" );
    return false;
  }

  return true;
}

bool SCChiselCodeGen::GenerateChisel(){

  if( !Parser ){
    Msgs->PrintMsg( L_ERROR, "No parser input" );
    return false;
  }

  // this should probably be deprecated
  if( ChiselFile.length() == 0 ){
    Msgs->PrintMsg( L_ERROR, "Chisel output file cannot be null" );
    return false;
  }

  // if it exists, read the signal map
  if( SigMap.length() > 0 ){
    SM = new CoreGenSigMap();
    if( !SM->ReadSigMap(SigMap) ){
      Msgs->PrintMsg( L_ERROR, "Error reading signal map" );
      return false;
    }
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
