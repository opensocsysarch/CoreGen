//
// _SCChiselCodeGen_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
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
  : Parser(P), Opts(O), Msgs(M), Perf(NULL), ChiselFile(COF), CSM(nullptr) {
  InitIntrinsics();
  InitPasses();
  Perf = new SCPerf(Msgs->GetStream());
}

SCChiselCodeGen::~SCChiselCodeGen(){
  Intrins.clear();
  Passes.clear();
  if( CSM )
    delete CSM;
  if( Perf )
    delete Perf;
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
  Intrins.push_back(static_cast<SCIntrin *>(new SCNop()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCIn()));
  Intrins.push_back(static_cast<SCIntrin *>(new SCOut()));
}

void SCChiselCodeGen::InitPasses(){
  Passes.push_back(static_cast<SCPass *>(new SCInstArg(SCParser::TheModule.get(),
                                                       Opts,
                                                       Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCInstFormat(SCParser::TheModule.get(),
                                                       Opts,
                                                       Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCIOWarn(SCParser::TheModule.get(),
                                                      Opts,
                                                      Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCFieldIO(SCParser::TheModule.get(),
                                                       Opts,
                                                       Msgs)));
  Passes.push_back(static_cast<SCPass *>(new SCMetadataAdjust(SCParser::TheModule.get(),
                                                              Opts,
                                                              Msgs)));
}

void SCChiselCodeGen::WriteChiselHeader(std::ofstream &out,
                                        std::string FName ){
  out << "//" << std::endl;
  out << "// " << FName << std::endl;
  out << "//" << std::endl;
  out << "// Chisel generated from StoneCutter input source" << std::endl;
  out << "// " << SCCurrentDateTime();
  out << "//" << std::endl << std::endl;
}

std::vector<std::string> SCChiselCodeGen::GetPassList(){
  std::vector<std::string> NameVect;
  std::vector<SCPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    SCPass *P = (*it);
    NameVect.push_back(P->GetName());
  }

  return NameVect;
}

std::vector<std::string> SCChiselCodeGen::GetOptsList(){
  std::vector<std::string> OptsVect;
  std::vector<SCPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    SCPass *P = (*it);
    OptsVect.push_back(P->GetOptions());
  }

  return OptsVect;
}

bool SCChiselCodeGen::ExecutePasses(){
  bool rtn = true;

  // make sure we don't run the passes twice
  if( Opts->IsPassRun() )
    return true;

  Opts->PassRun();

  std::map<std::string,std::string> PassOpts = Opts->GetSCPassOptions();
  std::map<std::string,std::string>::iterator mit;


  if( !Opts->IsDisableSCPass() && !Opts->IsEnableSCPass()
      && Opts->IsOptimize() ){
    // execute all the passes
    std::vector<SCPass *>::iterator it;
    for( it=Passes.begin(); it != Passes.end(); ++it ){
      SCPass *P = (*it);

      // set the pass options
      mit = PassOpts.find(P->GetName());
      if( mit != PassOpts.end() )
        P->SetExecOpts(mit->second);

      if( Opts->IsVerbose() ){
        Msgs->PrintRawMsg( "Executing StoneCutter Pass: " + P->GetName() );
      }
      if( !P->Execute() ){
        rtn = false;
      }
    }
  }else if( Opts->IsEnableSCPass() ){
    // manually enabled passes
    std::vector<std::string> E = Opts->GetEnableSCPass();
    std::vector<SCPass *>::iterator it;
    std::vector<std::string>::iterator str;
    for( it=Passes.begin(); it != Passes.end(); ++it ){
      SCPass *P = (*it);

      // set the pass options
      mit = PassOpts.find(P->GetName());
      if( mit != PassOpts.end() )
        P->SetExecOpts(mit->second);

      // check to see if the current pass is in our vector
      str = std::find(E.begin(),E.end(),P->GetName());
      if( str != E.end() ){
        // it resides in our list, execute it
        if( !P->Execute() ){
          rtn = false;
        }
      }
    }
  }else if( Opts->IsDisableSCPass() ){
    // manually disabled passes
    std::vector<std::string> D = Opts->GetDisabledSCPass();
    std::vector<SCPass *>::iterator it;
    std::vector<std::string>::iterator str;
    for( it=Passes.begin(); it != Passes.end(); ++it ){
      SCPass *P = (*it);

      // set the pass options
      mit = PassOpts.find(P->GetName());
      if( mit != PassOpts.end() )
        P->SetExecOpts(mit->second);

      // check to see if the current pass is in our vector
      str = std::find(D.begin(),D.end(),P->GetName());
      if( str == D.end() ){
        // it doesn't reside in our list, execute it
        if( !P->Execute() ){
          rtn = false;
        }
      }
    }
  }

  return rtn;
}

bool SCChiselCodeGen::IsRegEnabled( SCSig *Sig, std::string Field ){
  for( unsigned i=0; i<Sig->GetNumInputs(); i++ ){
    if( Sig->GetInput(i) == Field )
      return true;
  }
  return false;
}

bool SCChiselCodeGen::IsInteger(std::string Str){
  if( Str.length() == 0 )
    return false;
  return std::regex_match( Str, std::regex( ( "((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?" ) ) );
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
    OutFile << "| IMM_SEL | EN_IMM | ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_TARGET | uBr_ALTERNATE |" << std::endl;
  }else{
    OutFile << "| ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_TARGET | uBr_ALTERNATE |" << std::endl;
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
  OutFile << "// - MEM_WR = Determines is operation is read/write" << std::endl;
  OutFile << "// - EN_MEM = Memory enable signal **" << std::endl;
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << "// - uBr_TARGET = Primary uOp branch target (next operation)" << std::endl;
  OutFile << "// - uBr_ALTERNATE = Alternate uOp branch target (only used in branch operations)" << std::endl;
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << "// ** - Only one of these signals can be selected within a give uOp" << std::endl;
  OutFile << "//----------------------------------------------------------------" << std::endl;
  OutFile << std::endl << std::endl;
}

void SCChiselCodeGen::WriteFETCHUOp(SCPipeInfo *PInfo){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  //-- [MA <- PC]
  //-- [A <- PC]
  OutFile << "\t\tLabel(\"U_FETCH\"), Signals(Cat(LDIR_X, RS_PC, RWR_0, REN_1, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    if( i == 0 ){
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_1, ";
    }else{
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
    }
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_X, AEN_0, LDMA_1, MWR_X, MEN_0, UBR_N), \"X\", \"X\")" << std::endl;

  //-- [IR <- MEM]
  OutFile << "\t\t\tSignals(Cat(LDIR_1, RS_X, RWR_X, REN_0, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    if( i == 0 ){
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_0, ";
    }else{
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
    }
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_X, AEN_0, LDMA_0, MWR_0, MEN_1, UBR_S), \"X\", \"X\")" << std::endl;

  //-- [PC <- A + 4]
  OutFile << "\t\t\tSignals(Cat(LDIR_0, RS_X, RWR_1, REN_1, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    if( i == 0 ){
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_0, ";
    }else{
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
    }
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  // TODO : need to derive what the width of the instructions are
  OutFile << "ALU_INC_A_4, AEN_1, LDMA_X, MWR_X, MEN_0, UBR_D), \"X\", \"X\")" << std::endl;
}

void SCChiselCodeGen::WriteNOPUOp(SCPipeInfo *PInfo){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  //-- [uBr to FETCH]
  OutFile << "\t\t,Label(\"U_NOP\"), Signals(Cat(LDIR_0, RS_X, RWR_X, REN_0, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_X, AEN_0, LDMA_X, MWR_X, MEN_X, UBR_J), \"U_FETCH\", \"X\")" << std::endl;
}

void SCChiselCodeGen::WriteILLEGALUOp(SCPipeInfo *PInfo){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  //-- [uBr to FETCH]
  OutFile << "\t\t,Label(\"U_ILLEGAL\"), Signals(Cat(LDIR_0, RS_X, RWR_X, REN_0, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_X, AEN_0, LDMA_X, MWR_X, MEN_X, UBR_N), \"X\", \"X\")" << std::endl;

  OutFile << "\t\t\tSignals(Cat(LDIR_0, RS_PC, RWR_1, REN_1, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_EVEC, AEN_1, LDMA_X, MWR_X, MEN_0, UBR_J), \"U_FETCH\",\"X\")" << std::endl;
}

void SCChiselCodeGen::WriteUNIMPLUOp(SCPipeInfo *PInfo){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  //-- [uBr to FETCH]
  OutFile << "\t\t,Label(\"U_UNIMP\"), Signals(Cat(LDIR_0, RS_X, RWR_X, REN_0, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_X, AEN_0, LDMA_X, MWR_X, MEN_X, UBR_J), \"U_FETCH\",\"X\")" << std::endl;
}

void SCChiselCodeGen::WriteINITPCUOp(SCPipeInfo *PInfo){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  //-- [uBr to FETCH]
  OutFile << "\t\t,Label(\"U_INITPC\"), Signals(Cat(LDIR_0, RS_PC, RWR_1, REN_1, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_X, AEN_0, LDMA_X, MWR_X, MEN_X, UBR_J), \"U_FETCH\",\"X\")" << std::endl;
}

std::string SCChiselCodeGen::DecodeRegSlot( std::string &SigStr,
                                            SCPipeInfo *PInfo,
                                            bool &IsImm ){
  // inputs are in the form "FIELD_SIGTYPE"
  // we need to strip the field out
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(SigStr);
  while(std::getline(tokenStream,token,'_'))
    tokens.push_back(token);

  // now determine if this is an immediate value
  for( unsigned i=0; i<PInfo->GetNumUniqueImmFields(); i++ ){
    if( tokens[0] == PInfo->GetUniqueImmFieldName(i) )
      IsImm = true;
  }

  return tokens[0];
}

std::string SCChiselCodeGen::SCToUpper( std::string Str ){
  std::locale loc;
  std::string RtnStr;
  for (std::string::size_type i=0; i<Str.length(); ++i)
    RtnStr += std::toupper(Str[i],loc);

  return RtnStr;
}

void SCChiselCodeGen::EmitNOP(SCSig *Sig,
                              unsigned NumSigs,
                              unsigned NSig,
                              std::string &BrTarget,
                              std::string &UOpBr,
                              SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  //-- [uBr to FETCH]
  OutFile << "LDIR_0, RS_PC, RWR_1, REN_1, ";
  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    OutFile << "IS_X, IEN_0, ";
  }
  OutFile << "ALU_X, AEN_0, LDMA_X, MWR_X, MEN_X, ";

  // determine if this is the final uOp in the instruction
  if( IsFinalUOp( NumSigs, NSig ) ){
    BrTarget  = "U_FETCH";
    UOpBr     = "UBR_J";
  }
}

void SCChiselCodeGen::EmitSIGINSTF(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitALU_SLT(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitALU_SLTU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitALU_COPY(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitPC_INCR(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // this is the same as a NOP, just to FETCH
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
}

void SCChiselCodeGen::EmitALU_OP(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // Step 1: Query the inputs and look for any immediate fields and constants
  unsigned Constant = 0;      // unsigned constant integer; integers < 0 are converted to unsigned
  bool isConstant   = false;  // determines if we have a constant int
  bool isImmediate  = false;  // determines if we have an immediate field value
  bool isRegister   = false;  // determines if we have a register input
  std::string ImmEn;
  for( unsigned i=0; i<Sig->GetNumInputs(); i++ ){
    if(IsInteger( Sig->GetInput( i ))){
      std::string::size_type sz;
      Constant = (unsigned)(std::stol(Sig->GetInput(i),&sz));
      isConstant = true;
    }else if( PInfo->IsRegisterField(Sig->GetInput(i)) ){
      ImmEn = Sig->GetInput(i);
      isImmediate = true;
    }else if( PInfo->IsImmField(Sig->GetInput(i)) ){
      isRegister = true;
    }
  }

  // Step 2: write out the register selects
  if( isRegister ){
    OutFile << "LDIR_0, RS_RD, RWR_1, REN_1, ";
    for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
      if( IsRegEnabled( Sig, PInfo->GetUniqueFieldName(i) ))
        OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_1, ";
      else
        OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_0, ";
    }
  }else{
    OutFile << "LDIR_0, RS_X, RWR_X, REN_0, ";
    for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_0, ";
    }
  }

  // Step 3: write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    if( isImmediate ){
      OutFile << "IS_" << ImmEn << ", IEN_1, ";
    }else{
      OutFile << "IS_X, IEN_0, ";
    }
  }

  // Step 4: write out the ALU selects
  if( isConstant ){
    OutFile << Sig->SigTypeToStr() << "_" << std::to_string(Constant)
            << ", AEN_1, LDMA_X, MWR_X, MEN_0, ";
  }else{
    OutFile << Sig->SigTypeToStr() << ", AEN_1, LDMA_X, MWR_X, MEN_0, ";
  }

  // determine if this is the final uOp in the instruction
  if( IsFinalUOp( NumSigs, NSig ) ){
    BrTarget  = "U_FETCH";
    UOpBr     = "UBR_J";
  }
}


void SCChiselCodeGen::EmitPC_BRJMP(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitPC_JALR(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitBR_N(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
}

void SCChiselCodeGen::EmitBR_NE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_EQ(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_GT(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_GTU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_GE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_GEU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_LT(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_LTU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_LE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_LEU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &AltTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
  AltTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceFalse());
}

void SCChiselCodeGen::EmitBR_J(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
}

void SCChiselCodeGen::EmitBR_JR(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // emit a nop that includes the branch targets
  EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
  BrTarget = Sig->GetInst() + "."
              + std::to_string(signed(NSig) + Sig->GetDistanceTrue());
}

void SCChiselCodeGen::EmitMUX_NE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_EQ(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_GT(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_GTU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_GE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_GEU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_LT(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_LTU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_LE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX_LEU(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMUX(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitREG_READ(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
  // LABEL | LD_IR | REG_SEL | REG_WR | EN_REG
  // <LD_regfield,...>
  // <IMM_SEL,EN_IMM>
  // ALU_OP | EN_ALU | LD_MA | MEM_WR | EN_MEM | uBr_SEL | uBr_Target | uBr_Alternate

  // decode the register select
  bool IsImm = false;
  std::string FieldName = Sig->GetName();
  std::string Slot = DecodeRegSlot(FieldName,PInfo,IsImm);

  //-- [uBr to FETCH]
  if( !IsImm ){
    OutFile << "LDIR_0, RS_" << SCToUpper(Slot) << ", RWR_0, REN_1, ";
  }else{
    OutFile << "LDIR_0, RS_X, RWR_0, REN_0, ";
  }

  // write out the register selects
  for( unsigned i=0; i<PInfo->GetNumUniqueRegFields(); i++ ){
    if( !IsImm ){
      // write out a register select
      if( Slot == PInfo->GetUniqueFieldName(i) ){
        OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_1, ";
      }else{
        OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_0, ";
      }
    }else{
      // disable all the registers
      OutFile << "LD" << PInfo->GetUniqueFieldName(i) << "_X, ";
    }
  }
  // write out the immediate selects
  if( PInfo->GetNumUniqueImmFields() > 0 ){
    if( !IsImm ){
      OutFile << "IS_X, IEN_0, ";
    }else{
      OutFile << "IS_" << Slot << ", IEN_1, ";
    }
  }
  OutFile << "ALU_X, AEN_0, LDMA_X, MWR_X, MEN_0, ";

  // determine if this is the final uOp in the instruction
  if( IsFinalUOp( NumSigs, NSig ) ){
    BrTarget  = "U_FETCH";
    UOpBr     = "UBR_J";
  }
}

void SCChiselCodeGen::EmitREG_WRITE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitAREG_READ(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitAREG_WRITE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMEM_READ(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitMEM_WRITE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

void SCChiselCodeGen::EmitFENCE(SCSig *Sig,
                            unsigned NumSigs,
                            unsigned NSig,
                            std::string &BrTarget,
                            std::string &UOpBr,
                            SCPipeInfo *PInfo ){
}

inline bool SCChiselCodeGen::IsFinalUOp( unsigned NumSigs, unsigned NSig ){
  if( (NSig+1) == NumSigs )
    return true;
  return false;
}

void SCChiselCodeGen::WriteUOp( SCSig *Sig,
                                unsigned NumSigs,
                                unsigned NSig,
                                SCPipeInfo *PInfo ){
#if 0
  std::cout << "Writing "
            << Sigs[j]->GetInst() << ":"
            << Sigs[j]->GetName() << ":"
            << Sigs[j]->SigTypeToStr() << std::endl;
#endif
  // Step 1: write out instruction label
  if( NSig == 0 ){
    // write out the top-level instruction label
    OutFile << "\t\t,Label(\"" << Sig->GetInst() << "\"),Signals(Cat(";
  }else{
    // write out all subsequent instruction labels
    // this permits us to perform uOp branches within a target instruction
    OutFile << "\t\t,Label(\"" << Sig->GetInst() << "."
      << std::to_string(NSig) << "\"),Signals(Cat(";
  }

  // Step 2: decode the target uOp and write out the fields
  std::string BrTarget  = "X";     // the uOp branch target
  std::string AltTarget = "X";     // the uOp alternate target
  std::string UOpBr     = "UBR_N"; // the uOp branch type

  switch( Sig->GetType() ){
  case SIGINSTF:
    EmitSIGINSTF(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_ADD:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_SUB:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_SLL:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_SRL:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_SRA:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_AND:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_OR:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_XOR:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_SLT:
    EmitALU_SLT(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_SLTU:
    EmitALU_SLTU(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_COPY:
    EmitALU_COPY(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_MUL:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_DIV:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_REM:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_FADD:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_FSUB:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_FMUL:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_FDIV:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case ALU_FREM:
    EmitALU_OP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case PC_INCR:
    EmitPC_INCR(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case PC_BRJMP:
    EmitPC_BRJMP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case PC_JALR:
    EmitPC_JALR(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case BR_N:
    EmitBR_N(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case BR_NE:
    EmitBR_NE(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_EQ:
    EmitBR_EQ(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_GT:
    EmitBR_GT(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_GTU:
    EmitBR_GTU(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_GE:
    EmitBR_GE(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_GEU:
    EmitBR_GEU(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_LT:
    EmitBR_LT(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_LTU:
    EmitBR_LTU(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_LE:
    EmitBR_LE(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_LEU:
    EmitBR_LEU(Sig,NumSigs,NSig,BrTarget,AltTarget,UOpBr,PInfo);
    break;
  case BR_J:
    EmitBR_J(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case BR_JR:
    EmitBR_JR(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_NE:
    EmitMUX_NE(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_EQ:
    EmitMUX_EQ(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_GT:
    EmitMUX_GT(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_GTU:
    EmitMUX_GTU(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_GE:
    EmitMUX_GE(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_GEU:
    EmitMUX_GEU(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_LT:
    EmitMUX_LT(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_LTU:
    EmitMUX_LTU(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_LE:
    EmitMUX_LE(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX_LEU:
    EmitMUX_LEU(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MUX:
    EmitMUX(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case REG_READ:
    EmitREG_READ(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case REG_WRITE:
    EmitREG_WRITE(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case AREG_READ:
    EmitAREG_READ(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case AREG_WRITE:
    EmitAREG_WRITE(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MEM_READ:
    EmitMEM_READ(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case MEM_WRITE:
    EmitMEM_WRITE(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case FENCE:
    EmitFENCE(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  case SIGUNK:
  default:
    // write a NOP
    EmitNOP(Sig,NumSigs,NSig,BrTarget,UOpBr,PInfo);
    break;
  }

  // Step 3: write out the uOp branch logic
  OutFile << UOpBr << "), \"" << BrTarget
          << "\", \"" << AltTarget << "\")" << std::endl;
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
  WriteFETCHUOp(PInfo);
  WriteNOPUOp(PInfo);
  WriteILLEGALUOp(PInfo);
  WriteUNIMPLUOp(PInfo);
  WriteINITPCUOp(PInfo);

  // Stage 2: for each instruction in the signal map,
  //          walk the list of signals and generate the uOp
  //          table for each instruction
  bool rtn = true;
  if( CSM->GetNumInst() == 0 ){
    rtn = false;
    Msgs->PrintMsg( L_ERROR, "Signal map contains no instructions" );
  }else{
    std::vector<std::string> Insts = CSM->GetInstVect();
    for( unsigned i=0; i<Insts.size(); i++ ){
      std::vector<SCSig *> Sigs = CSM->GetSigVect(Insts[i]);
      for( unsigned j=0; j<Sigs.size(); j++ ){
        Perf->InsertUOp(Insts[i]);
        WriteUOp( Sigs[j], Sigs.size(), j, PInfo );
      }
    }
  }

  // write out the closure of the table structure
  OutFile << "\t)" << std::endl;
  OutFile << "}" << std::endl;

  // write the footer
  OutFile << "}" << std::endl;

  delete PInfo;

  // Build the microcode compiler
  if( !WriteUCodeCompiler() )
    rtn = false;

  return rtn;
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

bool SCChiselCodeGen::DeriveBitPat(std::string &BP){
  std::string CompFile = ChiselFile.substr(0,ChiselFile.find_last_of("\\/")) +
                          "/instructions.chisel";

  std::string token;
  std::ifstream BPStream(CompFile);
  if( !BPStream.is_open() ){
    Msgs->PrintMsg( L_ERROR, "Failed to open Chisel instructions file " + CompFile );
    return false;
  }

  while(std::getline(BPStream,token)){
    std::size_t pos = token.find("BitPat");
    if( pos != std::string::npos ){
      // found a potential match
      std::string token2 = token.substr(pos+8);
      BP = token2.substr(0,token2.length()-2);
      return true;
    }
  }

  BPStream.close();
  return false;
}

bool SCChiselCodeGen::WriteUCodeCompiler(){

  // top-level details
  std::string Package = Opts->GetPackage();
  std::string ISA = Opts->GetISA();

  if( Package.length() == 0 ){
    Package = ISA;
  }

  // construct the output file name
  std::string CompFile = ChiselFile.substr(0,ChiselFile.find_last_of("\\/")) +
                          "/microcodecompiler.scala";

  // open the output file
  std::ofstream LOutFile;
  LOutFile.open(CompFile, std::ios::trunc);
  if( !OutFile.is_open() ){
    Msgs->PrintMsg( L_ERROR, "Failed to open Chisel output file " + CompFile );
    return false;
  }

  // write out the file header
  WriteChiselHeader(LOutFile,CompFile);

  // write out the package includes
  LOutFile << "package " << Package << std::endl;
  LOutFile << "{" << std::endl << std::endl;

  LOutFile << "import chisel3._" << std::endl;
  LOutFile << "import chisel3.util._" << std::endl << std::endl;

  LOutFile << "import Common.Instructions._" << std::endl;
  LOutFile << "import scala.collection.mutable.ArrayBuffer" << std::endl << std::endl;

  LOutFile << "abstract class MicroOp()" << std::endl;
  LOutFile << "case class Label(name: String) extends MicroOp" << std::endl;
  LOutFile << "case class Signals(ctrl_bits: Bits, label: String = \"X\") extends MicroOp" << std::endl << std::endl;

  // write out the MicrocodeCompiler block
  // -- class block
  LOutFile << "object MicrocodeCompiler" << std::endl << "{" << std::endl << std::endl;

  // -- generateInstructionList
  std::string BitPat;
  if( !DeriveBitPat(BitPat) )
    return false;
  LOutFile << "\tdef generateInstructionList (): Map[String, BitPat] =" << std::endl;
  LOutFile << "\t{" << std::endl;
  LOutFile << "\t\tvar inst_list = Map[String, BitPat]()" << std::endl;
  LOutFile << "\t\tval instClass = Common.Instructions.getClass()" << std::endl;
  LOutFile << "\t\tval b = BitPat(\"" << BitPat << "\")" << std::endl;
  LOutFile << "\t\tval bitsClass = b.getClass()" << std::endl << std::endl;
  LOutFile << "\t\tfor (m <- instClass.getMethods())" << std::endl;
  LOutFile << "\t\t{" << std::endl;
  LOutFile << "\t\t\tval name = m.getName()" << std::endl;
  LOutFile << "\t\t\tval rtype = m.getReturnType()" << std::endl;
  LOutFile << "\t\t\tif (rtype == bitsClass)" << std::endl;
  LOutFile << "\t\t\t{" << std::endl;
  LOutFile << "\t\t\t\tval i = m.invoke(Common.Instructions)" << std::endl;
  LOutFile << "\t\t\t\tinst_list += ((name, i.asInstanceOf[BitPat]))" << std::endl;
  LOutFile << "\t\t\t}" << std::endl;
  LOutFile << "\t\t}" << std::endl << std::endl;
  LOutFile << "\t\treturn inst_list" << std::endl;
  LOutFile << "\t}" << std::endl << std::endl;

  // -- generateDispatchTable
  LOutFile << "\tdef generateDispatchTable (labelTargets: Map[String,Int]): "
           << "Array[(BitPat, UInt)]=" << std::endl;
  LOutFile << "\t{" << std::endl;
  LOutFile << "\t\tprintln(\"Generating Opcode Dispatch Table for ISA="
           << ISA << "\")" << std::endl;
  LOutFile << "\t\tvar dispatch_targets = ArrayBuffer[(BitPat, UInt)]()" << std::endl;
  LOutFile << "\t\tval inst_list        = generateInstructionList()" << std::endl << std::endl;
  LOutFile << "\t\tfor ((inst_str, inst_bits) <- inst_list)" << std::endl;
  LOutFile << "\t\t{" << std::endl;
  LOutFile << "\t\t\tif (labelTargets.contains(inst_str))" << std::endl;
  LOutFile << "\t\t\t{" << std::endl;
  LOutFile << "\t\t\t\tdispatch_targets += ((inst_bits -> labelTargets(inst_str).U))" << std::endl;
  LOutFile << "\t\t\t}" << std::endl;
  LOutFile << "\t\t}" << std::endl << std::endl;
  LOutFile << "\t\tvar unused_targets   = ArrayBuffer[String]()" << std::endl;
  LOutFile << "\t\tfor ((label_str, addr) <- labelTargets)" << std::endl;
  LOutFile << "\t\t{" << std::endl;
  LOutFile << "\t\t\tif (!inst_list.contains(label_str))" << std::endl;
  LOutFile << "\t\t\t\tunused_targets += label_str" << std::endl;
  LOutFile << "\t\t}" << std::endl << std::endl;
  LOutFile << "\t\tprintln(\"\")" << std::endl;
  LOutFile << "\t\tprintln(\"Unused Labels for Dispatching:\")" << std::endl;
  LOutFile << "\t\tprintln(\"    (Verify no instruction labels made it here by accident)\")" << std::endl;
  LOutFile << "\t\tprintln(\"\")" << std::endl;
  LOutFile << "\t\tprintln(\"    \" + unused_targets)" << std::endl;
  LOutFile << "\t\tprintln(\"\")" << std::endl << std::endl;
  LOutFile << "\t\treturn dispatch_targets.toArray" << std::endl;
  LOutFile << "\t}" << std::endl << std::endl;

  // -- contructLabelTargetMap
  LOutFile << "\tdef constructLabelTargetMap(uop_insts: Array[MicroOp]): (Map[String,Int], Int)  =" << std::endl;
  LOutFile << "\t{" << std::endl;
  LOutFile << "\t\tprintln(\"Building Microcode labelTargetMap for "
           << ISA
           << "\")" << std::endl;
  LOutFile << "\t\tvar label_map = Map[String,Int]()" << std::endl;
  LOutFile << "\t\tvar uaddr = 0" << std::endl;
  LOutFile << "\t\tfor (uop_inst <- uop_insts)" << std::endl;
  LOutFile << "\t\t{" << std::endl;
  LOutFile << "\t\t\tuop_inst match" << std::endl;
  LOutFile << "\t\t\t{" << std::endl;
  LOutFile << "\t\t\t\tcase Label(name)       => label_map += ((name, uaddr))" << std::endl;
  LOutFile << "\t\t\t\tcase Signals(code,str) => uaddr += 1" << std::endl;
  LOutFile << "\t\t\t}" << std::endl;
  LOutFile << "\t\t}" << std::endl << std::endl;
  LOutFile << "\t\tprintln(\"Label Map \" + label_map)" << std::endl;
  LOutFile << "\t\tprintln(\" MicroROM size     : \" + (uaddr-1) + \" lines\")" << std::endl;
  LOutFile << "\t\tprintln(\" Bitwidth of uAddr : \" + log2Ceil(uaddr-1) + \" bits\")" << std::endl;
  LOutFile << "\t\tprintln(\"\")" << std::endl;
  LOutFile << "\t\treturn (label_map, log2Ceil(uaddr-1))" << std::endl;
  LOutFile << "\t}" << std::endl << std::endl;

  // -- emitRomBits
  LOutFile << "\tdef emitRomBits(uop_lines: Array[MicroOp], labelTargets: "
           << "Map[String,Int], label_sz: Int): Array[Bits] =" << std::endl;
  LOutFile << "\t{" << std::endl;
  LOutFile << "\t\tvar buf = ArrayBuffer[Bits]()" << std::endl << std::endl;
  LOutFile << "\t\tfor (uop_line <- uop_lines)" << std::endl;
  LOutFile << "\t\t{" << std::endl;
  LOutFile << "\t\t\tuop_line match" << std::endl;
  LOutFile << "\t\t\t{" << std::endl;
  LOutFile << "\t\t\t\tcase Label(name) =>" << std::endl;
  LOutFile << "\t\t\t\tcase Signals(ctrl_bits, \"X\") =>" << std::endl;
  LOutFile << "\t\t\t\t\tval line = Cat(ctrl_bits, 0.U(label_sz.W))" << std::endl;
  LOutFile << "\t\t\t\t\tbuf += (line)" << std::endl << std::endl;
  LOutFile << "\t\t\t\tcase Signals(ctrl_bits, label) =>" << std::endl;
  LOutFile << "\t\t\t\t\tval line = Cat(ctrl_bits, labelTargets(label).U(label_sz.W))" << std::endl;
  LOutFile << "\t\t\t\t\tbuf += (line)" << std::endl;
  LOutFile << "\t\t\t}" << std::endl;
  LOutFile << "\t\t}" << std::endl;
  LOutFile << "\t\tprintln(\"\")" << std::endl;
  LOutFile << "\t\treturn buf.toArray" << std::endl;
  LOutFile << "\t}" << std::endl;

  // close the MicrocodeCompiler block
  LOutFile << std::endl << "}" << std::endl;

  // closing brace
  LOutFile << std::endl << "}" << std::endl;

  // close the output file
  LOutFile.close();

  return true;
}

bool SCChiselCodeGen::ExecuteVLIWPipelineOpt(CoreGenSigMap *SM){
  if( !SM )
    return false;

  SCVLIWPipeBuilder *PB = new SCVLIWPipeBuilder(SCParser::TheModule.get(),
                                                Opts,
                                                Msgs);

  if( !PB )
    return false;

  if( Opts->IsVerbose() ){
    Msgs->PrintRawMsg( "Executing StoneCutter Pass: " + PB->GetName() );
  }

  // set the options
  std::map<std::string,std::string> PassOpts = Opts->GetSCPassOptions();
  std::map<std::string,std::string>::iterator mit;
  mit = PassOpts.find(PB->GetName());
  if( mit != PassOpts.end() )
    PB->SetExecOpts(mit->second);

  // set the signal map
  if( !PB->SetSignalMap(SM) ){
    delete PB;
    return false;
  }

  // execute the pass
  bool rtn = true;
  if( !Opts->IsDisableSCPass() && !Opts->IsEnableSCPass() ){
    if( !PB->Execute() )
      rtn = false;
  }else if( Opts->IsEnableSCPass() ){
    // manually enabled passes
    std::vector<std::string> E = Opts->GetEnableSCPass();
    std::vector<std::string>::iterator str;
    str = std::find(E.begin(),E.end(),PB->GetName());
    if( str != E.end() ){
      if( !PB->Execute() )
        rtn = false;
    }

  }else if( Opts->IsDisableSCPass() ){
    // manually disabled passes
    std::vector<std::string> D = Opts->GetDisabledSCPass();
    std::vector<std::string>::iterator str;
    str = std::find(D.begin(),D.end(),PB->GetName());
    if( str == D.end() ){
      if( !PB->Execute() )
        rtn = false;
    }
  }

  // delete the signal map object
  delete PB;

  return rtn;
}

bool SCChiselCodeGen::ExecuteRISCPipelineOpt(CoreGenSigMap *SM){
  if( !SM )
    return false;

  SCPipeBuilder *PB = new SCPipeBuilder(SCParser::TheModule.get(),
                                        Opts,
                                        Msgs);
  if( !PB )
    return false;

  if( Opts->IsVerbose() ){
    Msgs->PrintRawMsg( "Executing StoneCutter Pass: " + PB->GetName() );
  }

  // set the options
  std::map<std::string,std::string> PassOpts = Opts->GetSCPassOptions();
  std::map<std::string,std::string>::iterator mit;
  mit = PassOpts.find(PB->GetName());
  if( mit != PassOpts.end() )
    PB->SetExecOpts(mit->second);

  // set the signal map
  if( !PB->SetSignalMap(SM) ){
    delete PB;
    return false;
  }

  // execute the pass
  bool rtn = true;
  if( !Opts->IsDisableSCPass() && !Opts->IsEnableSCPass() ){
    if( !PB->Execute() )
      rtn = false;
  }else if( Opts->IsEnableSCPass() ){
    // manually enabled passes
    std::vector<std::string> E = Opts->GetEnableSCPass();
    std::vector<std::string>::iterator str;
    str = std::find(E.begin(),E.end(),PB->GetName());
    if( str != E.end() ){
      if( !PB->Execute() )
        rtn = false;
    }

  }else if( Opts->IsDisableSCPass() ){
    // manually disabled passes
    std::vector<std::string> D = Opts->GetDisabledSCPass();
    std::vector<std::string>::iterator str;
    str = std::find(D.begin(),D.end(),PB->GetName());
    if( str == D.end() ){
      if( !PB->Execute() )
        rtn = false;
    }
  }

  // delete the signal map object
  delete PB;

  return rtn;
}

bool SCChiselCodeGen::ExecutePipelineOpt(CoreGenSigMap *SM){

  CoreGenSigMap *RSM = SM->SplitInstSigMap();
  CoreGenSigMap *VSM = SM->SplitVLIWSigMap();

  if( !ExecuteRISCPipelineOpt(RSM) )
    return false;

  if( !ExecuteVLIWPipelineOpt(VSM) )
    return false;

  SM->FuseSignalMaps(RSM,VSM);
  delete RSM;
  delete VSM;

  return true;
}

bool SCChiselCodeGen::ExecuteCodegen(){
  // Execute all the necessary passes
  if( !Opts->IsPassRun() ){
    if( !ExecutePasses() ){
      return false;
    }
  }

  // attempt to perform pipeline optimization
  if( CSM && Opts->IsPipeline() && !Opts->IsPassRun() ){
    if( !ExecutePipelineOpt(CSM) ){
      return false;
    }
  }

  // attempt to read the signal map back out
  if( CSM ){
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

  // attempt to perform pipeline optimization
  if( Opts->IsPipeline() ){
    CoreGenSigMap *TSM = new CoreGenSigMap();
    if( !TSM->ReadSigMap(SigMap) ){
      Msgs->PrintMsg( L_ERROR, "Could not read signal map for pipelining pass" );
      delete TSM;
      return false;
    }
    if( !ExecutePipelineOpt(TSM) ){
      return false;
    }
    delete TSM;
  }

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
    CSM = new CoreGenSigMap();
    if( !CSM->ReadSigMap(SigMap) ){
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
  WriteChiselHeader(OutFile,ChiselFile);

  // Execute the codegen
  if( !ExecuteCodegen() ){
    Msgs->PrintMsg( L_ERROR, "Failed to generate Chisel output" );
    OutFile.close();
    return false;
  }

  // close the output file
  OutFile.close();

  if( Opts->IsPerf() ){
    Perf->PrintStats();
  }

  return true;
}

// EOF
