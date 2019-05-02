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

const std::string SCSigMap::SigTypeToStr(SigType T){
  switch( T ){
    case SIGUNK:
      return "SIGUNK";
      break;
    case SIGINSTF:
      return "SIGINSTF";
      break;
    case ALU_ADD:
      return "ALU_ADD";
      break;
    case ALU_SUB:
      return "ALU_SUB";
      break;
    case ALU_SLL:
      return "ALU_SLL";
      break;
    case ALU_SRL:
      return "ALU_SRL";
      break;
    case ALU_SRA:
      return "ALU_SRA";
      break;
    case ALU_AND:
      return "ALU_AND";
      break;
    case ALU_OR:
      return "ALU_OR";
      break;
    case ALU_XOR:
      return "ALU_XOR";
      break;
    case ALU_SLT:
      return "ALU_SLT";
      break;
    case ALU_SLTU:
      return "ALU_SLTU";
      break;
    case ALU_COPY:
      return "ALU_COPY";
      break;
    case ALU_MUL:
      return "ALU_MUL";
      break;
    case ALU_DIV:
      return "ALU_DIV";
      break;
    case ALU_REM:
      return "ALU_REM";
      break;
    case ALU_FADD:
      return "ALU_FADD";
      break;
    case ALU_FSUB:
      return "ALU_FSUB";
      break;
    case ALU_FMUL:
      return "ALU_FMUL";
      break;
    case ALU_FDIV:
      return "ALU_FDIV";
      break;
    case ALU_FREM:
      return "ALU_FREM";
      break;
    case PC_INCR:
      return "PC_INCR";
      break;
    case PC_BRJMP:
      return "PC_BRJMP";
      break;
    case PC_JALR:
      return "PC_JALR";
      break;
    case BR_N:
      return "BR_N";
      break;
    case BR_NE:
      return "BR_NE";
      break;
    case BR_EQ:
      return "BR_EQ";
      break;
    case BR_GE:
      return "BR_GE";
      break;
    case BR_GEU:
      return "BR_GEU";
      break;
    case BR_LT:
      return "BR_LT";
      break;
    case BR_LTU:
      return "BR_LTU";
      break;
    case BR_J:
      return "BR_J";
      break;
    case BR_JR:
      return "BR_JR";
      break;
    case REG_READ:
      return "REG_READ";
      break;
    case REG_WRITE:
      return "REG_WRITE";
      break;
    case MEM_READ:
      return "MEM_READ";
      break;
    case MEM_WRITE:
      return "MEM_WRITE";
      break;
    default:
      return "SIGUNK";
      break;
  }
}

bool SCSigMap::WriteInstSignals(YAML::Emitter *out){
  if( out == nullptr )
    return false;

  *out << YAML::Key << "Instructions" << YAML::BeginSeq;

  // walk all the functions and generate the appropriate signals
  for( auto &Func : TheModule->getFunctionList() ){
    std::string Inst = Func.getName().str();
    std::vector<SigType> Sigs;

    *out << YAML::BeginMap << YAML::Key << "Inst" << YAML::Value << Inst;

    // walk all the known signals and derive the per-instruction
    // signal map
    for( unsigned i=0; i<Signals.size(); i++ ){
      if( Signals[i]->GetInst() == Inst )
        Sigs.push_back(Signals[i]->GetType());
    }

    // make the vector unique
    std::sort( Sigs.begin(), Sigs.end() );
    Sigs.erase( std::unique(Sigs.begin(),Sigs.end()), Sigs.end() );

    // write the individual vector signal map
    for( unsigned j=0; j<Sigs.size(); j++ ){
      *out << YAML::Key << "Signal" << YAML::Value << SigTypeToStr(Sigs[j]);
    }

    *out << YAML::EndMap;
  }

  *out << YAML::EndSeq;

  return true;
}

bool SCSigMap::WriteTopLevelSignals(YAML::Emitter *out){
  if( out == nullptr )
    return false;

  std::vector<SigType> Sigs;

  // retrieve all the signals
  for( unsigned i=0; i<Signals.size(); i++ ){
    Sigs.push_back(Signals[i]->GetType());
  }

  // make the vector unique
  std::sort( Sigs.begin(), Sigs.end() );
  Sigs.erase( std::unique(Sigs.begin(),Sigs.end()), Sigs.end() );

  // write each signal out
  *out << YAML::Key << "SignalTop" << YAML::BeginSeq << YAML::BeginMap;
  for( unsigned i=0; i<Sigs.size(); i++ ){
    *out << YAML::Key << "Signal" << YAML::Value << SigTypeToStr(Sigs[i]);
  }
  *out << YAML::EndMap << YAML::EndSeq;

  return true;
}

bool SCSigMap::WriteSigMap(){
  if( SigMap.length() == 0 ){
    this->PrintMsg( L_ERROR, "SigMap file name is null" );
    return false;
  }

  // open the file
  std::ofstream OutYaml(SigMap.c_str());

  /// open the yaml emitter
  YAML::Emitter out(OutYaml);

  // open the YAML stream
  out << YAML::BeginMap;

  // write the top-level signals
  if( !WriteTopLevelSignals(&out) ){
    this->PrintMsg( L_ERROR, "Failed to write top-level signals" );
    OutYaml.close();
    return false;
  }

  // write the individual instruction signals
  if( !WriteInstSignals(&out) ){
    this->PrintMsg( L_ERROR, "Failed to write individual instruction signals" );
    OutYaml.close();
    return false;
  }

  // end the YAML stream
  out << YAML::EndMap;

  // close the file
  OutYaml.close();

  return true;
}

bool SCSigMap::TranslateLogicalOp( Function &F,
                                   Instruction &I,
                                   SigType Type ){
  // initiate the binary signal
  Signals.push_back(new SCSig(Type,F.getName().str()));

  // interrogate the operands and write the operand enable signals
  if( !TranslateOperands(F,I) )
    return false;

  return true;
}

bool SCSigMap::TranslateBinaryOp( Function &F,
                                  Instruction &I,
                                  SigType Type ){
  // initiate the binary signal
  Signals.push_back(new SCSig(Type,F.getName().str()));

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
      Signals.push_back(new SCSig(REG_READ,F.getName().str(),OpName+"_READ"));
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
      Signals.push_back(new SCSig(REG_WRITE,F.getName().str(),WOpName+"_WRITE"));
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

SCSigMap::SCSig::SCSig(SigType T,std::string I,std::string N)
  : Type(T), Inst(I), Name(N){
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
