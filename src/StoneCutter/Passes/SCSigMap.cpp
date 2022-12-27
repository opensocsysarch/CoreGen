//
// _SCSigMap_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
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

  // set the VLIW flag
  Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);

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

  // set the VLIW flag
  Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);

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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
    // this->PrintMsg(L_MSG, "Intrin: " + Intrin->GetKeyword());
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
            // set the VLIW flag
            Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
          }else if(!isImm){
            // search for temporaries that match the instruction:irname mapping
            std::string TmpReg = Signals->GetTempMap(F.getName().str(),
                                                     Arg->get()->getName().str());
            // we only eject if the instruction is not VLIW.  VLIW stages are permitted
            // to pass temporaries between one another using arguments
            if( TmpReg.length() == 0 && !IsVLIWStage(F) ){
              // we cannot create a new temp on register read
              this->PrintMsg(L_ERROR,
                             "Cannot create temporary on register read for " +
                             Intrin->GetKeyword() );
              return false;
            }
            Signals->InsertSignal(new SCSig(AREG_READ,
                                            Width,
                                            F.getName().str(),
                                            TmpReg+"_READ",
                                            GetMDPipeName(I)));
            // set the VLIW flag
            Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
          }
        }// end for auto Arg
      }

      // Generate the logic signals for the intrinsic
      if( !Intrin->GetSigMap(Signals,I,F.getName().str()) ){
        this->PrintMsg(L_ERROR, Intrin->GetKeyword() + " intrinsic" + "\n\t----> " + Intrin->GetErrMsg());
        return false;
      }

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
          // set the VLIW flag
          Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
        }else if(!isWImm){
          std::string tmp = Signals->GetTempReg(F.getName().str(),
                                            LHS->getName().str(), Width );
          Signals->InsertSignal(new SCSig(AREG_WRITE,
                                          Width,
                                          F.getName().str(),
                                          tmp+"_WRITE",
                                          GetMDPipeName(I)));
          // set the VLIW flag
          Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
  bool rtn = false;
  rtn = Signals->InsertSignal(new SCSig(MUX,
                                         1,
                                         F.getName().str(),
                                         GetMDPipeName(I)));
  // set the VLIW flag
  Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
  return rtn;
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

signed SCSigMap::GetSwitchCaseDistance(Function &F, Instruction &BI, BasicBlock *Target ){
  // This is slightly different than traditional branch target calculation
  // Switch instructions with cases generate branch targets with BasicBlocks
  // that always follow the current BasicBlock.  In which case, we need to
  // start the search at our current BasicBlock and walk the instructions to the
  // appropriate BB target

  if( BI.getParent() == Target )
    return 0;

  bool enableCount = false;
  signed Count = 0;
  for( auto &BB : F.getBasicBlockList() ){
    if( BB.getName() == Target->getName() ){
      // found the target
      return Count+1;
    }
    for( auto &Inst : BB.getInstList() ){
      if( !F.isDeclaration() ){
        if( Inst.isIdenticalTo( &BI ) && (&BI == &Inst) ){
          // found the starting point
          enableCount = true;
        }else if( enableCount && !IsIgnoreInst(Inst) ){
          Count++;
        }
      }
    }
  }

  return 0; // we should never reach this point
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

  // debug messages
#if 0
  std::cout << "Function           = " << F.getName().str() << std::endl;
  std::cout << "Source Instruction = " << BI.getOpcodeName() << std::endl;
  std::cout << "Target Instruction = " << Target.getOpcodeName() << std::endl;
  std::cout << "Distance           = " << TargetID - SourceID << std::endl;
#endif
  return TargetID - SourceID;
}

bool SCSigMap::RecursiveBranchType(Function &F, Instruction &I,
                                   Value *V, SigType &Type){
  for( auto &BB : F.getBasicBlockList() ){
    if( !F.isDeclaration() ){
      for( auto &Inst : BB.getInstList() ){
        Value *LHS = cast<Value>(&Inst);
        if( LHS == V ){
          // found a possible match
          auto *CI = dyn_cast<CmpInst>(&Inst);
          if(CI){
            switch( CI->getPredicate() ){
            case CmpInst::FCMP_UNO:
            case CmpInst::FCMP_ORD:
            case CmpInst::FCMP_ONE:
            case CmpInst::FCMP_FALSE:
            case CmpInst::FCMP_UNE:
            case CmpInst::ICMP_NE:
              Type = BR_NE;
              break;
            case CmpInst::FCMP_TRUE:
            case CmpInst::FCMP_UEQ:
            case CmpInst::FCMP_OEQ:
            case CmpInst::ICMP_EQ:
              Type = BR_EQ;
              break;
            case CmpInst::FCMP_OGT:
            case CmpInst::ICMP_SGT:
              Type = BR_GT;
              break;
            case CmpInst::FCMP_OGE:
            case CmpInst::ICMP_SGE:
              Type = BR_GE;
              break;
            case CmpInst::FCMP_OLT:
            case CmpInst::ICMP_SLT:
              Type = BR_LT;
              break;
            case CmpInst::FCMP_OLE:
            case CmpInst::ICMP_SLE:
              Type = BR_LE;
              break;
            case CmpInst::FCMP_UGT:
            case CmpInst::ICMP_UGT:
              Type = BR_GTU;
              break;
            case CmpInst::FCMP_UGE:
            case CmpInst::ICMP_UGE:
              Type = BR_GEU;
              break;
            case CmpInst::FCMP_ULT:
            case CmpInst::ICMP_ULT:
              Type = BR_LTU;
              break;
            case CmpInst::FCMP_ULE:
            case CmpInst::ICMP_ULE:
              Type = BR_LEU;
              break;
            default:
              Type = BR_N;
              break;
            }
            return true;
          }//else, ignore it
        }
      }
    }
  }
  return false;
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
          if( !CI ){
            // this is not a compare instruction
            // in which case, we need to walk up the def-use tree
            // to find the last compare instruction with a target operand
            // used in this chain
            for( unsigned op=1; op < Inst.getNumOperands(); op++ ){
              Value *TmpV = Inst.getOperand(op);
              if( RecursiveBranchType(F,I,TmpV, Type) ){
                return Type;
              }
            }
          }else{
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
        (UncDist != 1) ){
      Signals->InsertSignal(new SCSig(BR_N,
                                      1,
                                      UncDist,
                                      0, // alternate branch is 0
                                      F.getName().str(),
                                      GetMDPipeName(I)));
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    }
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
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    }else if( !DTNull & DFNull ){
      // alternate branch is null, generate a single ended branch
      // this is effectively now an unconditional branch
      Signals->InsertSignal(new SCSig(GetBranchType(F,I),
                                      1,
                                      GetBranchDistance(F,I,BI->getSuccessor(0)->front()),
                                      0,  // alternate branch
                                      F.getName().str(),
                                      GetMDPipeName(I)));
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    }else if( DTNull & !DFNull ){
      // primary branch is null, generate a single ended branch with alternate as the target
      // this is effectively now an unconditional branch
      Signals->InsertSignal(new SCSig(GetBranchType(F,I),
                                      1,
                                      GetBranchDistance(F,I,BI->getSuccessor(1)->front()),
                                      0,  // alternate branch
                                      F.getName().str(),
                                      GetMDPipeName(I)));
      // set the VLIW flag
      Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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

bool SCSigMap::TranslateSwitch(Function &F, Instruction &I){
  // Examine the switch statement and create a set of mux signals based upon the
  // switch contents.
  // - mux_1 : first case
  // - mux_2 : second case
  // - ...
  // - mux_n : n'th case
  // - mux   : default branch
  auto *SI = dyn_cast<SwitchInst>(&I);
  signed CaseDistance = 0;

  // the branch stack includes two instructions (cmp+branch) for every case
  // statement and a single instruction (br) for the default statement
  // we decrement this value by two for every new case statement then the default
  // branch target uOp is simply the label target
  signed BranchStack = ((SI->getNumSuccessors()-1)*2)-1;

  for( SwitchInst::CaseIt it = SI->case_begin(), ie = SI->case_end();
       it != ie; ++it ){
    // insert compare
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);

    // insert conditional branch
    CaseDistance = GetSwitchCaseDistance(F,I,it->getCaseSuccessor()) + BranchStack;
    Signals->InsertSignal(new SCSig(BR_EQ,
                                    1,
                                    CaseDistance,
                                    1,    // fall through to the next case statement
                                    F.getName().str(),
                                    GetMDPipeName(I)));
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    BranchStack -= 2; // decrement for the next case statement
  }

  // insert the default branch
  // this will be unconditional as none of the previous case statements were met
  Signals->InsertSignal(new SCSig(BR_N,
                                  1,
                                  GetSwitchCaseDistance(F,I,SI->getDefaultDest()),
                                  0, // alternate branch is 0
                                  F.getName().str(),
                                  GetMDPipeName(I)));
  Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);

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
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_OGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_OGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_OLT:
    Signals->InsertSignal(new SCSig(MUX_LTU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_OLE:
    Signals->InsertSignal(new SCSig(MUX_LEU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_ONE:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_ORD:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_UNO:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_UEQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_UGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_UGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_ULT:
    Signals->InsertSignal(new SCSig(MUX_LTU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_ULE:
    Signals->InsertSignal(new SCSig(MUX_LEU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_UNE:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::FCMP_TRUE:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_EQ:
    Signals->InsertSignal(new SCSig(MUX_EQ,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_NE:
    Signals->InsertSignal(new SCSig(MUX_NE,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_UGT:
    Signals->InsertSignal(new SCSig(MUX_GTU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_UGE:
    Signals->InsertSignal(new SCSig(MUX_GEU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_ULT:
    Signals->InsertSignal(new SCSig(MUX_LTU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_ULE:
    Signals->InsertSignal(new SCSig(MUX_LEU,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_SGT:
    Signals->InsertSignal(new SCSig(MUX_GT,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_SGE:
    Signals->InsertSignal(new SCSig(MUX_GE,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_SLT:
    Signals->InsertSignal(new SCSig(MUX_LT,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  case CmpInst::ICMP_SLE:
    Signals->InsertSignal(new SCSig(MUX_LE,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
    break;
  default:
    Signals->InsertSignal(new SCSig(SIGUNK,1,F.getName().str(),GetMDPipeName(I)));
    // set the VLIW flag
    Signals->GetSignal(Signals->GetNumSignals()-1)->SetVLIW(VLIW);
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
  //DEBUG: this->PrintMsg( L_WARN, "Generating signals for Func:Op = " +
  //                F.getName().str() + ":" + std::string(I.getOpcodeName()) );
  switch( I.getOpcode() ){
    // binary signals
  case Instruction::Add :
    if( !TranslateBinaryOp(F,I,ALU_ADD) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary Add operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::FAdd :
    if( !TranslateBinaryOp(F,I,ALU_FADD) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary FAdd operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Sub :
    if( !TranslateBinaryOp(F,I,ALU_SUB) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary Sub operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::FSub :
    if( !TranslateBinaryOp(F,I,ALU_FSUB) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary FSub operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Mul :
    if( !TranslateBinaryOp(F,I,ALU_MUL) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary Mul operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::FMul :
    if( !TranslateBinaryOp(F,I,ALU_FMUL) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary FMul operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::UDiv :
    if( !TranslateBinaryOp(F,I,ALU_DIV) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary UDiv operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::SDiv :
    if( !TranslateBinaryOp(F,I,ALU_DIV) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary SDiv operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::FDiv :
    if( !TranslateBinaryOp(F,I,ALU_FDIV) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary FDiv operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::URem :
    if( !TranslateBinaryOp(F,I,ALU_REM) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary URem operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::SRem :
    if( !TranslateBinaryOp(F,I,ALU_REM) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary SRem operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::FRem :
    if( !TranslateBinaryOp(F,I,ALU_FREM) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate binary FRem operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
    // logical signals
  case Instruction::Shl :
    if( !TranslateLogicalOp(F,I,ALU_SLL) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate logical Shl operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::LShr :
    if( !TranslateLogicalOp(F,I,ALU_SRL) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate logical LShl operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::AShr :
    if( !TranslateLogicalOp(F,I,ALU_SRA) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate logical Ahl operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::And :
    if( !TranslateLogicalOp(F,I,ALU_AND) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate logical And operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Or :
    if( !TranslateLogicalOp(F,I,ALU_OR) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate logical Or operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Xor :
    if( !TranslateLogicalOp(F,I,ALU_XOR) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate logical Xor operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
    // memory access signals
  case Instruction::Load :
  case Instruction::Store :
    if( !TranslateMemOp(F,I) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate memory operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
    // cast signals
  case Instruction::ZExt :
    break;
  case Instruction::SExt :
    break;
    // other signals (cmp, etc)
  case Instruction::ICmp :
    if( !TranslateCmpOp(F,I) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate ICmp operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::FCmp :
    if( !TranslateCmpOp(F,I) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate FCmp operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Call :
    if( !TranslateCallSig(F,I) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate Call operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Select :
    if( !TranslateSelectSig(F,I) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate Select operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Br :
    if( !TranslateBranch(F,I) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate Branch operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
    break;
  case Instruction::Switch :
    if( !TranslateSwitch(F,I) ){
      this->PrintMsg( L_ERROR,
                      "Failed to translate Switch operation at Instruction=" +
                      std::string(I.getOpcodeName()) + " within Function=" +
                      F.getName().str() );
      return false;
    }
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
    //DEBUG: this->PrintMsg( L_WARN, "SigMap Func = " + Func.getName().str() );
    for( auto &BB : Func.getBasicBlockList() ){
      // determine whether we need to handle VLIW staging
      if( IsVLIWStage(Func) ){
        VLIW = true;
      }else{
        VLIW = false;
      }
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
    if( !Func.isDeclaration() && !IsVLIWStage(Func) ){
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
