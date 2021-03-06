//
// _SCSigMap_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCSigMap.h"

SCSigMap::SCSigMap(Module *TM,
                           SCOpts *O,
                           SCMsg *M)
  : SCPass("SigMap","",TM,O,M) {
}

SCSigMap::~SCSigMap(){
}

bool SCSigMap::SetSignalMapFile(std::string SM){
  if( SM.length() == 0 ){
    this->PrintMsg( L_ERROR, "Signal map file is null" );
    return false;
  }
  SigMap = SM;
  return true;
}

bool SCSigMap::SetIntrins(std::vector<SCIntrin *>* I){
  if( I == nullptr )
    return false;

  Intrins = I;
  return true;
}

bool SCSigMap::TranslateLogicalOp( Function &F,
                                   Instruction &I,
                                   SigType Type ){
  // initiate the binary signal
  Signals->InsertSignal(new SCSig(Type,
                                  1,
                                  F.getName().str(),
                                  GetMDPipeName(I)));

  // we have the new signal, now we need to insert the target input requirements
  // by default, we use the last signal utilized
  if(!TranslateALUOperands(F,I,Signals->GetSignal(Signals->GetNumSignals()-1)))
    return false;

  // interrogate the operands and write the operand enable signals
  if( !TranslateTargetOperands(F,I) )
    return false;

  return true;
}

bool SCSigMap::TranslateALUOperands( Function &F,
                                     Instruction &I,
                                     SCSig *Sig ){
  bool isPredef = false;
  bool isImm = false;
  unsigned Width = 0;
  for( unsigned i=0; i<I.getNumOperands(); i++ ){
    Sig->InsertInput( TraceOperand(F,I.getOperand(i),isPredef,isImm,Width) );
    Sig->SetWidth(Width);
  }

  return true;
}

bool SCSigMap::TranslateBinaryOp( Function &F,
                                  Instruction &I,
                                  SigType Type ){
  // initiate the binary signal
  Signals->InsertSignal(new SCSig(Type,
                                  1,
                                  F.getName().str(),
                                  GetMDPipeName(I)));

  // we have the new signal, now we need to insert the target input requirements
  // by default, we use the last signal utilized
  if(!TranslateALUOperands(F,I,Signals->GetSignal(Signals->GetNumSignals()-1)))
    return false;

  // interrogate the operands and write the operand enable signals
  if( !TranslateTargetOperands(F,I) )
    return false;

  return true;
}

bool SCSigMap::IsLegalMemOp( Function &F,
                             Instruction &I ){
  if( I.getOpcode() == Instruction::Store ){
    // store operations need to trace the target (op 1) of the instruction
    bool isPredef = false;
    bool isImm = false;
    unsigned Width = 0;
    std::string WOpName = TraceOperand(F,I.getOperand(1),isPredef,isImm,Width);
    DataLayout* DL = new DataLayout(TheModule);
    auto *SInst = dyn_cast<StoreInst>(&I);
    Value *LV = SInst->getPointerOperand();
    PointerType *PT = cast<PointerType>(LV->getType());
    Width = DL->getTypeStoreSize(PT->getPointerElementType()) * 8;  // convert to bits
    delete DL;
    if( isPredef  ){
      Signals->InsertSignal(new SCSig(REG_WRITE,
                                      Width,
                                      F.getName().str(),
                                      WOpName+"_WRITE",
                                      GetMDPipeName(I)));
      return true;
    }
  }else if( I.getOpcode() == Instruction::Load ){
    // load operations need to trace the address of the source (op 0)
    bool isPredef = false;
    bool isImm = false;
    unsigned Width = 0;
    std::string WOpName = TraceOperand(F,I.getOperand(0),isPredef,isImm,Width);

    DataLayout* DL = new DataLayout(TheModule);
    auto *LInst = dyn_cast<LoadInst>(&I);
    Value *LV = LInst->getPointerOperand();
    PointerType *PT = cast<PointerType>(LV->getType());
    Width = DL->getTypeStoreSize(PT->getPointerElementType()) * 8;  // convert to bits
    delete DL;
    if( isPredef ){
      Signals->InsertSignal(new SCSig(REG_READ,
                                      Width,
                                      F.getName().str(),
                                      WOpName+"_READ",
                                      GetMDPipeName(I)));
      return true;
    }
  }

  return false;
}

bool SCSigMap::TranslateMemOp( Function &F,
                               Instruction &I ){
  if( I.getOpcode() == Instruction::Store ){
    // store operations need to trace the target (op 1) of the instruction
    bool isPredef = false;
    bool isImm = false;
    unsigned Width = 0;
    std::string WOpName = TraceOperand(F,I.getOperand(1),isPredef,isImm,Width);
    DataLayout* DL = new DataLayout(TheModule);
    auto *SInst = dyn_cast<StoreInst>(&I);
    Value *LV = SInst->getPointerOperand();
    PointerType *PT = cast<PointerType>(LV->getType());
    Width = DL->getTypeStoreSize(PT->getPointerElementType()) * 8;  // convert to bits
    delete DL;
    if( isPredef  ){
      Signals->InsertSignal(new SCSig(REG_WRITE,
                                      Width,
                                      F.getName().str(),
                                      WOpName+"_WRITE",
                                      GetMDPipeName(I)));
    }
  }else if( I.getOpcode() == Instruction::Load ){
    // load operations need to trace the address of the source (op 0)
    bool isPredef = false;
    bool isImm = false;
    unsigned Width = 0;
    std::string WOpName = TraceOperand(F,I.getOperand(0),isPredef,isImm,Width);

    DataLayout* DL = new DataLayout(TheModule);
    auto *LInst = dyn_cast<LoadInst>(&I);
    Value *LV = LInst->getPointerOperand();
    PointerType *PT = cast<PointerType>(LV->getType());
    Width = DL->getTypeStoreSize(PT->getPointerElementType()) * 8;  // convert to bits
    delete DL;
    if( isPredef ){
      Signals->InsertSignal(new SCSig(REG_READ,
                                      Width,
                                      F.getName().str(),
                                      WOpName+"_READ",
                                      GetMDPipeName(I)));
    }
  }else{
    this->PrintMsg( L_ERROR, "Encountered a memory operation that is not a Load or Store operation" );
    return false;
  }

  return true;
}

bool SCSigMap::TranslateTargetOperands( Function &F, Instruction &I ){
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
      Signals->InsertSignal(new SCSig(REG_WRITE,
                                      Width,
                                      F.getName().str(),
                                      WOpName+"_WRITE",
                                      GetMDPipeName(I)));
    }else if(!isWImm){
      // create a temporary register
#if 0
      std::cout << "create a temp : " << LHS->getName().str()
                << " from " << F.getName().str() << " of width " << Width << std::endl;
#endif
      std::string tmp = Signals->GetTempReg(F.getName().str(),
                                            LHS->getName().str(), Width );
      Signals->InsertSignal(new SCSig(AREG_WRITE,
                                      Width,
                                      F.getName().str(),
                                      tmp+"_WRITE",
                                      GetMDPipeName(I)));
    }
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
      Signals->InsertSignal(new SCSig(REG_READ,
                                      Width,
                                      F.getName().str(),
                                      OpName+"_READ",
                                      GetMDPipeName(I)));
    }else if(!isImm){
      // search for temporaries that match the instruction:irname mapping
      std::string TmpReg = Signals->GetTempMap(F.getName().str(),
                                               Op->get()->getName().str());
      if( TmpReg.length() == 0 ){
        // we cannot create a new temp on register read
        this->PrintMsg( L_ERROR, "Cannot create temporary registers for register read signals" );
        return false;
      }
      Signals->InsertSignal(new SCSig(AREG_READ,
                                      Width,
                                      F.getName().str(),
                                      TmpReg+"_READ",
                                      GetMDPipeName(I)));
    }
  }

  return TranslateTargetOperands(F,I);
}

bool SCSigMap::TranslateCallSig(Function &F, Instruction &I){
  // retrieve the name of the called function
  auto *CInst = dyn_cast<CallInst>(&I);
  std::string Callee = CInst->getCalledFunction()->getName().str();

  // walk the intrinsic vector and determine which
  // intrinsic is found
  for( auto i : *Intrins ){
    SCIntrin *Intrin = i;
    if( Intrin->GetKeyword() == Callee ){
      // found a matching intrinsic

      // Check to see whether the intrinsic implements a fused op.
      // If it does, we DO NOT want to look at the individual signal arguments
      // In which case, we need to look at the prior signal to fuse
      if( !Intrin->IsFusedOp() ){
        // Translate the arguments to the necessary signals
        for( auto Arg = CInst->arg_begin(); Arg != CInst->arg_end(); ++Arg){
          bool isPredef = false;
          bool isImm = false;
          unsigned Width = 0;

          std::string OpName = TraceOperand(F,Arg->get(),isPredef,isImm,Width);

          if( isPredef ){
            Signals->InsertSignal(new SCSig(REG_READ,
                                            Width,
                                            F.getName().str(),
                                            OpName+"_READ",
                                            GetMDPipeName(I)));
          }else if(!isImm){
            // search for temporaries that match the instruction:irname mapping
            std::string TmpReg = Signals->GetTempMap(F.getName().str(),
                                                     Arg->get()->getName().str());
            if( TmpReg.length() == 0 ){
              // we cannot create a new temp on register read
              return false;
            }
            Signals->InsertSignal(new SCSig(AREG_READ,
                                            Width,
                                            F.getName().str(),
                                            TmpReg+"_READ",
                                            GetMDPipeName(I)));
          }
        }// end for auto Arg
      }

      // Generate the logic signals for the intrinsic
      if( !Intrin->GetSigMap(Signals,I,F.getName().str()) )
        return false;

      // Walk the output arg and generate the write-enable intrinsics
      if( CInst->hasName() ){
        bool isWPredef = false;
        bool isWImm = false;
        unsigned Width = 0;
        Value *LHS = cast<Value>(CInst);

        std::string WOpName = TraceOperand(F,LHS,isWPredef,isWImm,Width);
        if( isWPredef && !isWImm ){
          Signals->InsertSignal(new SCSig(REG_WRITE,
                                          Width,
                                          F.getName().str(),
                                          WOpName+"_WRITE",
                                          GetMDPipeName(I)));
        }else if(!isWImm){
          std::string tmp = Signals->GetTempReg(F.getName().str(),
                                            LHS->getName().str(), Width );
          Signals->InsertSignal(new SCSig(AREG_WRITE,
                                          Width,
                                          F.getName().str(),
                                          tmp+"_WRITE",
                                          GetMDPipeName(I)));
        }// else, cannot write to immediates
      }
      return true;
    }
  }

  // no intrinsic was found for the callee
  this->PrintMsg( L_ERROR, "No intrinsic found for " + Callee );
  return false;
}

bool SCSigMap::TranslateSelectSig(Function &F, Instruction &I ){
  return Signals->InsertSignal(new SCSig(MUX,
                                         1,
                                         F.getName().str(),
                                         GetMDPipeName(I)));
}

bool SCSigMap::IsNullBranchTarget(Instruction &I){
  if( I.getOpcode() == Instruction::Ret ){
    //std::cout << "Null branch in Inst=" << std::string(I.getOpcodeName()) << std::endl;
    return true;
  }
  return false;
}

bool SCSigMap::IsIgnoreInst(Instruction &I){
  switch( I.getOpcode() ){
  case Instruction::ZExt :    // temporary
  case Instruction::SExt :    // temporary
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
  return false;
}

signed SCSigMap::GetBranchDistance(Function &F, Instruction &BI, Instruction &Target ){
  signed SourceID   = 0;
  signed TargetID   = 0;
  signed Count      = 0;

  if( BI.isIdenticalTo(&Target) )
    return 0;

  for( auto &BB : F.getBasicBlockList() ){
    for( auto &Inst : BB.getInstList() ){
      if( !F.isDeclaration() ){
        // walk all the instructions
        if( Inst.isIdenticalTo( &BI )  && (&BI == &Inst) ){
          SourceID = Count;
        }else if( Inst.isIdenticalTo( &Target ) && (&Target == &Inst) ){
          TargetID = Count;
        }

        if( !IsIgnoreInst(Inst) && IsLegalMemOp(F,Inst) )
          ++Count;
      }
    }
  }

#if 0
  // debug messages
  std::cout << "Function           = " << F.getName().str() << std::endl;
  std::cout << "Source Instruction = " << BI.getOpcodeName() << std::endl;
  std::cout << "Target Instruction = " << Target.getOpcodeName() << std::endl;
  std::cout << "Distance           = " << TargetID - SourceID << std::endl;
#endif

  return TargetID - SourceID;
}

SigType SCSigMap::GetBranchType(Function &F, Instruction &I){
  SigType Type = BR_N;          // default type
  Value *V = I.getOperand(0);   // conditional branch var

  // Walk all the basic blocks and instructions in the current function
  // (inst def) and find the instruction that writes the compare instruction
  // that determines the value used for the branch
  for( auto &BB : F.getBasicBlockList() ){
    if( !F.isDeclaration() ){
      // walk all the instructions
      for( auto &Inst : BB.getInstList() ){
        Value *LHS = cast<Value>(&Inst);
        if( V == LHS ){
          // found the initial cmp operation
          auto *CI = dyn_cast<CmpInst>(&Inst);

          switch( CI->getPredicate() ){
          case CmpInst::FCMP_UNO:
          case CmpInst::FCMP_ORD:
          case CmpInst::FCMP_ONE:
          case CmpInst::FCMP_FALSE:
          case CmpInst::FCMP_UNE:
          case CmpInst::ICMP_NE:
            return BR_NE;
            break;
          case CmpInst::FCMP_TRUE:
          case CmpInst::FCMP_UEQ:
          case CmpInst::FCMP_OEQ:
          case CmpInst::ICMP_EQ:
            return BR_EQ;
            break;
          case CmpInst::FCMP_OGT:
          case CmpInst::ICMP_SGT:
            return BR_GT;
            break;
          case CmpInst::FCMP_OGE:
          case CmpInst::ICMP_SGE:
            return BR_GE;
            break;
          case CmpInst::FCMP_OLT:
          case CmpInst::ICMP_SLT:
            return BR_LT;
            break;
          case CmpInst::FCMP_OLE:
          case CmpInst::ICMP_SLE:
            return BR_LE;
            break;
          case CmpInst::FCMP_UGT:
          case CmpInst::ICMP_UGT:
            return BR_GTU;
            break;
          case CmpInst::FCMP_UGE:
          case CmpInst::ICMP_UGE:
            return BR_GEU;
            break;
          case CmpInst::FCMP_ULT:
          case CmpInst::ICMP_ULT:
            return BR_LTU;
            break;
          case CmpInst::FCMP_ULE:
          case CmpInst::ICMP_ULE:
            return BR_LEU;
            break;
          default:
            return BR_N;
            break;
          }
        }
      }
    }
  }
  return Type;
}

bool SCSigMap::TranslateBranch(Function &F, Instruction &I){
  auto *BI = dyn_cast<BranchInst>(&I);

  // determine the type of branch so we can calculate the target
  if( BI->isUnconditional() ){
    //
    // Unconditional Branch
    //
    // If the branch is null, then the result will never be used in the instruction
    // implementation.  If its null, ignore the branch.  This is likely an unconditional 
    // branch to a 'ret' instruction (which will be ignored).
    // If the unconditional branch target distance is +1, then this branch
    // is unnecessary.
    // If the unconditional branch is not null and the distance is > 1,
    // then insert a new signal.
    signed UncDist = GetBranchDistance(F,I,BI->getSuccessor(0)->front());
    if( (!IsNullBranchTarget(BI->getSuccessor(0)->front())) &&
        (UncDist != 1) )
      Signals->InsertSignal(new SCSig(BR_N,
                                      1,
                                      UncDist,
                                      0, // alternate branch is 0
                                      F.getName().str(),
                                      GetMDPipeName(I)));
  }else{
    //
    // Conditional Branch
    //
    bool DTNull = IsNullBranchTarget(BI->getSuccessor(0)->front());
    bool DFNull = IsNullBranchTarget(BI->getSuccessor(1)->front());

    if( !DTNull && !DFNull ){
      // both branches are not null, generate two-ended branch
      Signals->InsertSignal(new SCSig(GetBranchType(F,I),
                                      1,
                                      GetBranchDistance(F,I,BI->getSuccessor(0)->front()),
                                      GetBranchDistance(F,I,BI->getSuccessor(1)->front()),
                                      F.getName().str(),
                                      GetMDPipeName(I)));
    }else if( !DTNull & DFNull ){
      // alternate branch is null, generate a single ended branch
      // this is effectively now an unconditional branch
      Signals->InsertSignal(new SCSig(GetBranchType(F,I),
                                      1,
                                      GetBranchDistance(F,I,BI->getSuccessor(0)->front()),
                                      0,  // alternate branch
                                      F.getName().str(),
                                      GetMDPipeName(I)));
    }else if( DTNull & !DFNull ){
      // primary branch is null, generate a single ended branch with alternate as the target
      // this is effectively now an unconditional branch
      Signals->InsertSignal(new SCSig(GetBranchType(F,I),
                                      1,
                                      GetBranchDistance(F,I,BI->getSuccessor(1)->front()),
                                      0,  // alternate branch
                                      F.getName().str(),
                                      GetMDPipeName(I)));
    }else{
      // both branches are null, return an error
      this->PrintMsg( L_ERROR, "Primary and alternate branch targets are unused uOps: " + 
                               BI->getSuccessor(0)->getName().str() + ":" +
                               BI->getSuccessor(1)->getName().str());
      return false;
    }
  }

  return true;
}

bool SCSigMap::TranslateCmpOp(Function &F, Instruction &I){
  // Examine the first operand of the compare instruction
  // and extract the appropriate type of signal
  auto *CI = dyn_cast<CmpInst>(&I);

  switch( CI->getPredicate() ){
  case CmpInst::FCMP_FALSE:
    break;
  case CmpInst::FCMP_OEQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_OGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_OGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_OLT:
    Signals->InsertSignal(new SCSig(MUX_LTU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_OLE:
    Signals->InsertSignal(new SCSig(MUX_LEU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_ONE:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_ORD:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_UNO:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_UEQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_UGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_UGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_ULT:
    Signals->InsertSignal(new SCSig(MUX_LTU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_ULE:
    Signals->InsertSignal(new SCSig(MUX_LEU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_UNE:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::FCMP_TRUE:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_EQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_NE:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_UGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_UGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_ULT:
    Signals->InsertSignal(new SCSig(MUX_LTU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_ULE:
    Signals->InsertSignal(new SCSig(MUX_LEU,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_SGT:
    Signals->InsertSignal(new SCSig(MUX_GT,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_SGE:
    Signals->InsertSignal(new SCSig(MUX_GE,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_SLT:
    Signals->InsertSignal(new SCSig(MUX_LT,1,F.getName().str(),GetMDPipeName(I)));
    break;
  case CmpInst::ICMP_SLE:
    Signals->InsertSignal(new SCSig(MUX_LE,1,F.getName().str(),GetMDPipeName(I)));
    break;
  default:
    Signals->InsertSignal(new SCSig(SIGUNK,1,F.getName().str(),GetMDPipeName(I)));
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
  case Instruction::Br :
    if( !TranslateBranch(F,I) )
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
    this->PrintMsg( L_ERROR, "Failed to decode instruction type: "
                    + std::string(I.getOpcodeName()) );
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
    Signals->InsertSignal(new SCSig(PC_INCR,
                                    1,
                                    F.getName().str(),
                                    "PC_INCR",
                                    ""));
  }else{
    // create PCJmp signal
    Signals->InsertSignal(new SCSig(PC_BRJMP,
                                    1,
                                    F.getName().str(),
                                    "PC_BRJMP",
                                    ""));
  }

  if( !Rtn )
    this->PrintMsg( L_ERROR, "Failed to discover the PC signal requirements" );

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
            this->PrintMsg( L_ERROR,
                            "Failed to generate signal map in instruction=" +
                            Func.getName().str() );
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

  // Stage 3: execute all the signal map passes
  if( !Signals->ExecutePasses() ){
    this->PrintMsg( L_ERROR, "Failed to execute signal map passes: " + Signals->GetErrStr() );
    delete Signals;
    return false;
  }

  // Stage 4: write the signal map out to a yaml file
  if( !Signals->WriteSigMap(SigMap) ){
    this->PrintMsg( L_ERROR, "Failed to write the signal map to the file " +
                    SigMap + " : " + Signals->GetErrStr() );
    delete Signals;
    return false;
  }

  delete Signals;
  return true;
}

// EOF
