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

bool SCSigMap::WriteSigMap(){
  return true;
}

bool SCSigMap::TranslateLogicalOp( Function &F,
                                   Instruction &I,
                                   SigType Type ){
  // initiate the binary signal
  Signals.push_back(new SCSig(Type,F.getName().str()));

  // interrogate the operands and write the operand enable signals
  return true;
}

bool SCSigMap::TranslateBinaryOp( Function &F,
                                  Instruction &I,
                                  SigType Type ){
  // initiate the binary signal
  Signals.push_back(new SCSig(Type,F.getName().str()));

  if( !TranslateOperands(F,I) )
    return false;

  return true;
}

bool SCSigMap::TranslateOperands( Function &F, Instruction &I ){

  // walk all the instruction operands and trace the operands
  // back to the origin; generate the necessary signals from there
  for( auto Op = I.op_begin(); Op != I.op_end(); ++Op){
    bool isPredef = false;
    std::string OpName;
    OpName = TraceOperand(F,Op,isPredef);
  }


  // interrogate the operands and write the operand enable signals
  for( unsigned i=0; i<I.getNumOperands(); i++ ){
    Value *Op = I.getOperand(i);
    // check to see if this is a predefined register/register class/field
    if( (HasGlobalAttribute(Op->getName().str(),"register")) ||
        (HasGlobalAttribute(Op->getName().str(),"subregister")) ||
        (HasGlobalAttribute(Op->getName().str(),"regclass")) ){
    }else if( (HasGlobalAttribute(Op->getName().str(),"fieldtype")) ){
      // part of an instruction format
      // check to see whether this is an encoding field or an immediate
    }else{
      // top-level construct not found, must be a local operand
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

bool SCSigMap::DiscoverSigMap(){
  bool Rtn = true;
  // Walk all the functions
  for( auto &Func : TheModule->getFunctionList() ){
    // walk all the basic blocks
    for( auto &BB : Func.getBasicBlockList() ){
      // walk all the instructions
      for( auto &Inst : BB.getInstList() ){
        if( !CheckSigReq(Func,Inst) ){
          Rtn = false;
        }
      }
    }
  }

  return Rtn;
}

bool SCSigMap::isALUSig( SigType T ){
  if( (T>SIGINSTF) && (T<PC_INCR) )
    return true;
  return false;
}

bool SCSigMap::isMemSig( SigType T ){
  if( T>REG_WRITE )
    return true;
  return false;
}

bool SCSigMap::isRegSig( SigType T ){
  if( (T>BR_JR) && (T<MEM_READ) )
    return true;
  return false;
}

bool SCSigMap::isBranchSig( SigType T ){
  if( (T>PC_JALR) && (T<REG_READ) )
    return true;
  return false;
}

bool SCSigMap::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

  // Stage 1: walk the module construct and discover any potential signals
  if( !DiscoverSigMap() ){
    return false;
  }

  // Stage 2: write the signal map out to a yaml file
  if( !WriteSigMap() ){
    return false;
  }

  return true;
}

SCSigMap::SCSig::SCSig(SigType T) : Type(T), SigWidth(0){
}

SCSigMap::SCSig::SCSig(SigType T,unsigned W) : Type(T), SigWidth(W){
}

SCSigMap::SCSig::SCSig(SigType T,std::string I) : Type(T), Inst(I){
}

SCSigMap::SCSig::SCSig(SigType T,unsigned W,std::string I)
  : Type(T), SigWidth(W), Inst(I){
}

SCSigMap::SCSig::SCSig(SigType T,unsigned W,std::string I,std::string N)
  : Type(T), SigWidth(W), Inst(I), Name(N){
}

SCSigMap::SCSig::~SCSig(){
}

bool SCSigMap::SCSig::SetType( SigType T ){
  switch( T ){
  case SIGUNK:
  case SIGINSTF:
  case ALU_ADD:
  case ALU_SUB:
  case ALU_SLL:
  case ALU_SRL:
  case ALU_SRA:
  case ALU_AND:
  case ALU_OR:
  case ALU_XOR:
  case ALU_SLT:
  case ALU_SLTU:
  case ALU_COPY:
  case ALU_MUL:
  case ALU_DIV:
  case ALU_REM:
  case ALU_FADD:
  case ALU_FSUB:
  case ALU_FMUL:
  case ALU_FDIV:
  case ALU_FREM:
  case PC_INCR:
  case PC_BRJMP:
  case PC_JALR:
  case BR_N:
  case BR_NE:
  case BR_EQ:
  case BR_GE:
  case BR_GEU:
  case BR_LT:
  case BR_LTU:
  case BR_J:
  case BR_JR:
  case REG_READ:
  case REG_WRITE:
  case MEM_READ:
  case MEM_WRITE:
    Type = T;
    return true;
  default:
    return false;
  }
  return true;
}

bool SCSigMap::SCSig::SetWidth( unsigned W ){
  SigWidth = W;
  return true;
}

bool SCSigMap::SCSig::SetName( std::string N ){
  Name = N;
  return true;
}

bool SCSigMap::SCSig::SetInst( std::string I ){
  Inst = I;
  return true;
}

// EOF
