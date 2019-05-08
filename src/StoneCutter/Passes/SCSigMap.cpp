//
// _SCSigMap_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCSigMap.h"

SCSigMap::SCSigMap(Module *TM,
                           SCOpts *O,
                           SCMsg *M)
  : SCPass("SigMap",TM,O,M) {
}

SCSigMap::~SCSigMap(){
}

bool SCSigMap::SetSignalMapFile(std::string SM){
  if( SM.length() == 0 ){
    return false;
  }
  SigMap = SM;
  return true;
}

bool SCSigMap::TranslateLogicalOp( Function &F,
                                   Instruction &I,
                                   SigType Type ){
  // initiate the binary signal
  Signals->InsertSignal(new SCSig(Type,F.getName().str()));

  // interrogate the operands and write the operand enable signals
  if( !TranslateOperands(F,I) )
    return false;

  return true;
}

bool SCSigMap::TranslateBinaryOp( Function &F,
                                  Instruction &I,
                                  SigType Type ){
  // initiate the binary signal
  Signals->InsertSignal(new SCSig(Type,F.getName().str()));

  // interrogate the operands and write the operand enable signals
  if( !TranslateOperands(F,I) )
    return false;

  return true;
}

bool SCSigMap::TranslateOperands( Function &F, Instruction &I ){

  // Note: this function only works for operations where we have:
  // NAME = LHS <op> RHS

  // walk all the instruction operands and trace the operands
  // back to the origin; generate the necessary signals from there
  for( auto Op = I.op_begin(); Op != I.op_end(); ++Op){
    bool isPredef = false;
    bool isImm = false;
    std::string OpName = TraceOperand(F,Op->get(),isPredef,isImm);
    // generate a signal if the source is register
    if( isPredef ){
      Signals->InsertSignal(new SCSig(REG_READ,F.getName().str(),OpName+"_READ"));
    }else if(!isImm){
      // create a temporary register
    }
  }

  // now examine the LHS of the operation (instruction name)
  // to generate the write signals
  if( I.hasName() ){
    bool isWPredef = false;
    bool isWImm = false;
    Value *LHS = cast<Value>(&I);
    std::string WOpName = TraceOperand(F,LHS,isWPredef,isWImm);
    if( isWPredef ){
      Signals->InsertSignal(new SCSig(REG_WRITE,F.getName().str(),WOpName+"_WRITE"));
    }else{
      // create a temporary register
    }
  }

  return true;
}

bool SCSigMap::CheckSigReq( Function &F, Instruction &I ){
  // Decode each one of the relevant operations.
  // The decoding map can be found in Instruction.def from
  // https://llvm.org/svn/llvm-project/llvm/trunk/include/llvm/IR/Instruction.def
  //
  // For each one of the relevant signals decoded, generate a signal map
  // and push it into the master vector of signals

  // Decode everything else
  switch( I.getOpcode() ){
    // binary signals
  case Instruction::Add :
    if( !TranslateBinaryOp(F,I,ALU_ADD) )
      return false;
    break;
  case Instruction::FAdd :
    if( !TranslateBinaryOp(F,I,ALU_FADD) )
      return false;
    break;
  case Instruction::Sub :
    if( !TranslateBinaryOp(F,I,ALU_SUB) )
      return false;
    break;
  case Instruction::FSub :
    if( !TranslateBinaryOp(F,I,ALU_FSUB) )
      return false;
    break;
  case Instruction::Mul :
    if( !TranslateBinaryOp(F,I,ALU_MUL) )
      return false;
    break;
  case Instruction::FMul :
    if( !TranslateBinaryOp(F,I,ALU_FMUL) )
      return false;
    break;
  case Instruction::UDiv :
    if( !TranslateBinaryOp(F,I,ALU_DIV) )
      return false;
    break;
  case Instruction::SDiv :
    if( !TranslateBinaryOp(F,I,ALU_DIV) )
      return false;
    break;
  case Instruction::FDiv :
    if( !TranslateBinaryOp(F,I,ALU_FDIV) )
      return false;
    break;
  case Instruction::URem :
    if( !TranslateBinaryOp(F,I,ALU_REM) )
      return false;
    break;
  case Instruction::SRem :
    if( !TranslateBinaryOp(F,I,ALU_REM) )
      return false;
    break;
  case Instruction::FRem :
    if( !TranslateBinaryOp(F,I,ALU_FREM) )
      return false;
    break;
    // logical signals
  case Instruction::Shl :
    if( !TranslateLogicalOp(F,I,ALU_SLL) )
      return false;
    break;
  case Instruction::LShr :
    if( !TranslateLogicalOp(F,I,ALU_SRL) )
      return false;
    break;
  case Instruction::AShr :
    if( !TranslateLogicalOp(F,I,ALU_SRA) )
      return false;
    break;
  case Instruction::And :
    if( !TranslateLogicalOp(F,I,ALU_AND) )
      return false;
    break;
  case Instruction::Or :
    if( !TranslateLogicalOp(F,I,ALU_OR) )
      return false;
    break;
  case Instruction::Xor :
    if( !TranslateLogicalOp(F,I,ALU_XOR) )
      return false;
    break;
    // memory access signals
  case Instruction::Alloca :
    break;
  case Instruction::Load :
    break;
  case Instruction::Store :
    break;
  case Instruction::GetElementPtr :
    break;
  case Instruction::Fence :
    break;
  case Instruction::AtomicCmpXchg :
    break;
  case Instruction::AtomicRMW :
    break;
    // cast signals
  case Instruction::Trunc :
    break;
  case Instruction::ZExt :
    break;
  case Instruction::SExt :
    break;
  case Instruction::FPToUI :
    break;
  case Instruction::FPToSI :
    break;
  case Instruction::UIToFP :
    break;
  case Instruction::SIToFP :
    break;
  case Instruction::FPTrunc :
    break;
  case Instruction::FPExt :
    break;
  case Instruction::PtrToInt :
    break;
  case Instruction::IntToPtr :
    break;
  case Instruction::BitCast :
    break;
  case Instruction::AddrSpaceCast :
    break;
  case Instruction::CleanupPad :
    break;
  case Instruction::CatchPad :
    break;
    // other signals (cmp, etc)
  case Instruction::ICmp :
    break;
  case Instruction::FCmp :
    break;
  case Instruction::Call :
    break;
  case Instruction::Select :
    break;
  case Instruction::ExtractElement :
    break;
  case Instruction::InsertElement :
    break;
  case Instruction::ShuffleVector :
    break;
  case Instruction::ExtractValue :
    break;
  case Instruction::InsertValue :
    break;
  case Instruction::Ret :
  case Instruction::PHI :
  case Instruction::UserOp1 :
  case Instruction::UserOp2 :
  case Instruction::VAArg :
  case Instruction::LandingPad :
    // ignore these instructions
    return true;
    break;
  default:
    return false;
    break;
  }
  return true;
}

bool SCSigMap::TranslatePCSig(Instruction &I,
                              bool &PCJump){
  // walk all the operands and investigate to see whether
  // their values are PC registers
  bool PC = false;
  for( auto Op = I.op_begin(); Op != I.op_end(); ++Op){
    Value *V = Op->get();
    if( HasGlobalAttribute(V->getName().str(),"register") &&
        HasGlobalAttribute(V->getName().str(),"pc") ){
      PC = true;
    }
  }

  PCJump = PC;

  return true;
}

bool SCSigMap::CheckPCReq(Function &F){
  bool Rtn = true;
  bool PCJump = false;
  // walk all the basic blocks and instructions
  // disover any relevant PC-related signals
  // if none are found, create an automatic
  // PCIncr signal, otherwise create a PCJmp signal
  for( auto &BB : F.getBasicBlockList() ){
    for( auto &Inst : BB.getInstList() ){
      bool LPC = false;
      if( !TranslatePCSig(Inst,LPC) )
        Rtn = false;

      if( LPC )
        PCJump = true;
    }
  }

  // create the signals
  if( !PCJump ){
    // create PCIncr signal
    Signals->InsertSignal(new SCSig(PC_INCR,F.getName().str(),"PC_INCR"));
  }else{
    // create PCJmp signal
    Signals->InsertSignal(new SCSig(PC_BRJMP,F.getName().str(),"PC_BRJMP"));
  }

  return Rtn;
}

bool SCSigMap::DiscoverSigMap(){
  bool Rtn = true;
  // Walk all the functions
  for( auto &Func : TheModule->getFunctionList() ){
    // walk all the basic blocks
    for( auto &BB : Func.getBasicBlockList() ){
      if( !Func.isDeclaration() ){
      // walk all the instructions
        for( auto &Inst : BB.getInstList() ){
          if( !CheckSigReq(Func,Inst) ){
            Rtn = false;
          }
        }
      }
    }

    // check the target function for explicit PC signals
    if( !Func.isDeclaration() ){
      if( !CheckPCReq(Func) ){
        Rtn = false;
      }
    }
  }

  return Rtn;
}

bool SCSigMap::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // Stage 1: Create a new CoreGenSigMap object
  Signals = new CoreGenSigMap();

  // Stage 2: walk the module construct and discover any potential signals
  if( !DiscoverSigMap() ){
    delete Signals;
    return false;
  }

  // Stage 3: write the signal map out to a yaml file
  if( !Signals->WriteSigMap(SigMap) ){
    delete Signals;
    return false;
  }

  delete Signals;
  return true;
}

// EOF
