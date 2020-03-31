//
// _SCSig_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenSigMap/SCSig.h"

SCSig::SCSig(SigType T)
  : Type(T), SigWidth(0), DistanceTrue(0), DistanceFalse(0){
  Name = this->SigTypeToStr();
}

SCSig::SCSig(SigType T,unsigned W)
  : Type(T), FType(FOP_UNK), SigWidth(W), DistanceTrue(0), DistanceFalse(0){
  Name = this->SigTypeToStr();
}

SCSig::SCSig(SigType T,std::string I)
  : Type(T), FType(FOP_UNK), SigWidth(0),
    DistanceTrue(0), DistanceFalse(0), Inst(I){
  Name = this->SigTypeToStr();
}

SCSig::SCSig(SigType T,unsigned W,std::string I,std::string P)
  : Type(T), FType(FOP_UNK), SigWidth(W),
    DistanceTrue(0), DistanceFalse(0), Inst(I),PipeName(P){
  Name = this->SigTypeToStr();
}

SCSig::SCSig(SigType T,std::string I,std::string N)
  : Type(T), FType(FOP_UNK), SigWidth(0),
    DistanceTrue(0), DistanceFalse(0), Inst(I), Name(N){
}

SCSig::SCSig(SigType T,unsigned W,signed DT,signed DF,std::string I,std::string P)
  : Type(T), FType(FOP_UNK), SigWidth(W),
    DistanceTrue(DT), DistanceFalse(DF), Inst(I), PipeName(P){
  Name = this->SigTypeToStr();
}

SCSig::SCSig(SigType T,unsigned W,std::string I,std::string N,std::string P)
  : Type(T), FType(FOP_UNK), SigWidth(W),
    DistanceTrue(0), DistanceFalse(0), Inst(I), Name(N), PipeName(P){
}

SCSig::SCSig(SigType T,unsigned W,signed DT,signed DF,
             std::string I,std::string N,std::string P)
  : Type(T), FType(FOP_UNK), SigWidth(W),
    DistanceTrue(DT), DistanceFalse(DF), Inst(I), Name(N), PipeName(P){
}

SCSig::~SCSig(){
}

bool SCSig::InsertInput( std::string Input ){
  // only insert unique input names
  auto it = std::find( Inputs.begin(), Inputs.end(), Input );
  if( it == Inputs.end() )
    Inputs.push_back(Input);
  return true;
}

std::string SCSig::GetInput( unsigned Idx ){
  if( Idx > (Inputs.size()-1) )
    return "";
  return Inputs[Idx];
}

bool SCSig::SetDistanceTrue( signed D ){
  DistanceTrue = D;
  return true;
}

bool SCSig::SetDistanceFalse( signed D ){
  DistanceFalse = D;
  return true;
}

bool SCSig::SetFusedType( FusedOpType T ){
  switch( T ){
  case FOP_UNK:
  case FOP_SEXT:
  case FOP_ZEXT:
  case FOP_NOT:
  case FOP_NEG:
    FType = T;
    return true;
    break;
  default:
    return false;
    break;
  }
}

bool SCSig::SetType( SigType T ){
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
  case BR_GT:
  case BR_GTU:
  case BR_GE:
  case BR_GEU:
  case BR_LT:
  case BR_LTU:
  case BR_LE:
  case BR_LEU:
  case BR_J:
  case BR_JR:
  case MUX_NE:
  case MUX_EQ:
  case MUX_GT:
  case MUX_GTU:
  case MUX_GE:
  case MUX_GEU:
  case MUX_LT:
  case MUX_LTU:
  case MUX_LE:
  case MUX_LEU:
  case MUX:
  case REG_READ:
  case REG_WRITE:
  case AREG_READ:
  case AREG_WRITE:
  case MEM_READ:
  case MEM_WRITE:
  case FENCE:
    Type = T;
    return true;
  default:
    return false;
  }
}

bool SCSig::SetWidth( unsigned W ){
  SigWidth = W;
  return true;
}

bool SCSig::IsPipeDefined(){
  if( PipeName.length() > 0 )
    return true;
  return false;
}

bool SCSig::SetPipeName( std::string PName ){
  PipeName = PName;
  return true;
}

bool SCSig::SetName( std::string N ){
  Name = N;
  return true;
}

bool SCSig::SetInst( std::string I ){
  Inst = I;
  return true;
}

bool SCSig::isALUSig(){
  if( (Type>SIGINSTF) && (Type<PC_INCR) )
    return true;
  return false;
}

bool SCSig::isMemSig(){
  if( Type>AREG_WRITE )
    return true;
  return false;
}

bool SCSig::isRegSig(){
  if( (Type>MUX) && (Type<MEM_READ) )
    return true;
  return false;
}

bool SCSig::isBranchSig(){
  if( (Type>PC_JALR) && (Type<REG_READ) )
    return true;
  return false;
}

const std::string SCSig::FusedOpTypeToStr(){
  switch( FType ){
  case FOP_SEXT:
    return "FOP_SEXT";
    break;
  case FOP_ZEXT:
    return "FOP_ZEXT";
    break;
  case FOP_NOT:
    return "FOP_NOT";
    break;
  case FOP_NEG:
    return "FOP_NEG";
    break;
  case FOP_UNK:
    break;
  default:
    return "FOP_UNK";
    break;
  }
  return "FOP_UNK";
}

const std::string SCSig::SigTypeToStr(){
  switch( Type ){
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
    case BR_GT:
      return "BR_GT";
    case BR_GTU:
      return "BR_GTU";
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
    case BR_LE:
      return "BR_LE";
      break;
    case BR_LEU:
      return "BR_LEU";
      break;
    case BR_J:
      return "BR_J";
      break;
    case BR_JR:
      return "BR_JR";
      break;
    case MUX_NE:
      return "MUX_NE";
      break;
    case MUX_EQ:
      return "MUX_EQ";
      break;
    case MUX_GT:
      return "MUX_GT";
      break;
    case MUX_GTU:
      return "MUX_GTU";
      break;
    case MUX_GE:
      return "MUX_GE";
      break;
    case MUX_GEU:
      return "MUX_GEU";
      break;
    case MUX_LT:
      return "MUX_LT";
      break;
    case MUX_LTU:
      return "MUX_LTU";
      break;
    case MUX_LE:
      return "MUX_LE";
      break;
    case MUX_LEU:
      return "MUX_LEU";
      break;
    case MUX:
      return "MUX";
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
    case AREG_READ:
      return "AREG_READ";
      break;
    case AREG_WRITE:
      return "AREG_WRITE";
      break;
    case MEM_WRITE:
      return "MEM_WRITE";
      break;
    case FENCE:
      return "FENCE";
      break;
    default:
      return "SIGUNK";
      break;
  }
}

// EOF
