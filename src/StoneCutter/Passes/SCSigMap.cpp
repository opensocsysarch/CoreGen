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

bool SCSigMap::TranslateMemOp( Function &F,
                               Instruction &I ){
  if( I.getOpcode() == Instruction::Store ){
    // store operations need to trace the target (op 1) of the instruction
    bool isPredef = false;
    bool isImm = false;
    unsigned Width = 0;
    std::string WOpName = TraceOperand(F,I.getOperand(1),isPredef,isImm,Width);
    if( isPredef ){
      Signals->InsertSignal(new SCSig(REG_WRITE,F.getName().str(),WOpName+"_WRITE"));
    }
  }else if( I.getOpcode() == Instruction::Load ){
    // load operations need to trace the address of the source (op 0)
    bool isPredef = false;
    bool isImm = false;
    unsigned Width = 0;
    std::string WOpName = TraceOperand(F,I.getOperand(0),isPredef,isImm,Width);
    if( isPredef ){
      Signals->InsertSignal(new SCSig(REG_READ,F.getName().str(),WOpName+"_READ"));
    }
  }else{
    return false;
  }

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
    unsigned Width = 0;
#if 0
    std::cout << "Tracing " << Op->get()->getName().str()
      << " from Func:Inst "
      << F.getName().str() << ":" << I.getName().str() << std::endl;
#endif
    std::string OpName = TraceOperand(F,Op->get(),isPredef,isImm,Width);
    // generate a signal if the source is register
    if( isPredef ){
      Signals->InsertSignal(new SCSig(REG_READ,F.getName().str(),OpName+"_READ"));
    }else if(!isImm){
      // search for temporaries that match the instruction:irname mapping
      std::string TmpReg = Signals->GetTempMap(F.getName().str(),
                                               Op->get()->getName().str());
      if( TmpReg.length() == 0 ){
        // we cannot create a new temp on register read
        return false;
      }
      Signals->InsertSignal(new SCSig(AREG_READ,F.getName().str(),TmpReg+"_READ"));
    }
  }

  // now examine the LHS of the operation (instruction name)
  // to generate the write signals
  if( I.hasName() ){
    bool isWPredef = false;
    bool isWImm = false;
    unsigned Width = 0;
    Value *LHS = cast<Value>(&I);
#if 0
    std::cout << "Tracing target " << LHS->getName().str()
      << " from Func:Inst "
      << F.getName().str() << ":" << I.getName().str() << std::endl;
#endif
    std::string WOpName = TraceOperand(F,LHS,isWPredef,isWImm,Width);
    if( isWPredef && !isWImm ){
      Signals->InsertSignal(new SCSig(REG_WRITE,F.getName().str(),WOpName+"_WRITE"));
    }else if(!isWImm){
      // create a temporary register
#if 0
      std::cout << "create a temp : " << LHS->getName().str()
                << " from " << F.getName().str() << " of width " << Width << std::endl;
#endif
      std::string tmp = Signals->GetTempReg(F.getName().str(),
                                            LHS->getName().str(), Width );
      Signals->InsertSignal(new SCSig(AREG_WRITE,F.getName().str(),tmp+"_WRITE"));
    }
  }

  return true;
}

bool SCSigMap::TranslateCallSig(Function &F, Instruction &I){
  return true;
}

bool SCSigMap::TranslateSelectSig(Function &F, Instruction &I ){
  return Signals->InsertSignal(new SCSig(MUX,F.getName().str()));
}

bool SCSigMap::TranslateCmpOp(Function &F, Instruction &I){
  // Examine the first operand of the compare instruction
  // and extract the appropriate type of signal
  auto *CI = dyn_cast<CmpInst>(&I);

  switch( CI->getPredicate() ){
  case CmpInst::FCMP_FALSE:
    break;
  case CmpInst::FCMP_OEQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,F.getName().str()));
    break;
  case CmpInst::FCMP_OGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,F.getName().str()));
    break;
  case CmpInst::FCMP_OGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,F.getName().str()));
    break;
  case CmpInst::FCMP_OLT:
    Signals->InsertSignal(new SCSig(MUX_LTU,F.getName().str()));
    break;
  case CmpInst::FCMP_OLE:
    Signals->InsertSignal(new SCSig(MUX_LEU,F.getName().str()));
    break;
  case CmpInst::FCMP_ONE:
    Signals->InsertSignal(new SCSig(MUX_EQ,F.getName().str()));
    break;
  case CmpInst::FCMP_ORD:
    Signals->InsertSignal(new SCSig(MUX_NE,F.getName().str()));
    break;
  case CmpInst::FCMP_UNO:
    Signals->InsertSignal(new SCSig(MUX_NE,F.getName().str()));
    break;
  case CmpInst::FCMP_UEQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,F.getName().str()));
    break;
  case CmpInst::FCMP_UGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,F.getName().str()));
    break;
  case CmpInst::FCMP_UGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,F.getName().str()));
    break;
  case CmpInst::FCMP_ULT:
    Signals->InsertSignal(new SCSig(MUX_LTU,F.getName().str()));
    break;
  case CmpInst::FCMP_ULE:
    Signals->InsertSignal(new SCSig(MUX_LEU,F.getName().str()));
    break;
  case CmpInst::FCMP_UNE:
    Signals->InsertSignal(new SCSig(MUX_NE,F.getName().str()));
    break;
  case CmpInst::FCMP_TRUE:
    Signals->InsertSignal(new SCSig(MUX_EQ,F.getName().str()));
    break;
  case CmpInst::ICMP_EQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,F.getName().str()));
    break;
  case CmpInst::ICMP_NE:
    Signals->InsertSignal(new SCSig(MUX_NE,F.getName().str()));
    break;
  case CmpInst::ICMP_UGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,F.getName().str()));
    break;
  case CmpInst::ICMP_UGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,F.getName().str()));
    break;
  case CmpInst::ICMP_ULT:
    Signals->InsertSignal(new SCSig(MUX_LTU,F.getName().str()));
    break;
  case CmpInst::ICMP_ULE:
    Signals->InsertSignal(new SCSig(MUX_LEU,F.getName().str()));
    break;
  case CmpInst::ICMP_SGT:
    Signals->InsertSignal(new SCSig(MUX_GT,F.getName().str()));
    break;
  case CmpInst::ICMP_SGE:
    Signals->InsertSignal(new SCSig(MUX_GE,F.getName().str()));
    break;
  case CmpInst::ICMP_SLT:
    Signals->InsertSignal(new SCSig(MUX_LT,F.getName().str()));
    break;
  case CmpInst::ICMP_SLE:
    Signals->InsertSignal(new SCSig(MUX_LE,F.getName().str()));
    break;
  default:
    Signals->InsertSignal(new SCSig(SIGUNK,F.getName().str()));
    break;
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
  case Instruction::Load :
  case Instruction::Store :
    if( !TranslateMemOp(F,I) )
      return false;
    break;
    // cast signals
  case Instruction::ZExt :
    break;
  case Instruction::SExt :
    break;
    // other signals (cmp, etc)
  case Instruction::ICmp :
    if( !TranslateCmpOp(F,I) )
      return false;
    break;
  case Instruction::FCmp :
    if( !TranslateCmpOp(F,I) )
      return false;
    break;
  case Instruction::Call :
    if( !TranslateCallSig(F,I) )
      return false;
    break;
  case Instruction::Select :
    if( !TranslateSelectSig(F,I) )
      return false;
    break;
  case Instruction::FPToUI :
  case Instruction::FPToSI :
  case Instruction::UIToFP :
  case Instruction::SIToFP :
  case Instruction::FPTrunc :
  case Instruction::FPExt :
  case Instruction::BitCast :
  case Instruction::PtrToInt :
  case Instruction::IntToPtr :
  case Instruction::ExtractElement :
  case Instruction::InsertElement :
  case Instruction::ExtractValue :
  case Instruction::InsertValue :
  case Instruction::Trunc :
  case Instruction::Fence :
  case Instruction::AtomicCmpXchg :
  case Instruction::AtomicRMW :
  case Instruction::ShuffleVector :
  case Instruction::AddrSpaceCast :
  case Instruction::CleanupPad :
  case Instruction::CatchPad :
  case Instruction::GetElementPtr :
  case Instruction::Alloca :
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
