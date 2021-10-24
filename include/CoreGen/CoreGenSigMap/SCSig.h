//
// _SCSig_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCSig
 *
 * \ingroup CoreGenSigMap
 *
 * \brief CoreGen Signal Map Type
 *
 */

#ifndef _SCSIG_H_
#define _SCSIG_H_

#include <string>
#include <vector>
#include <algorithm>

typedef enum{
  // Generic fused ops
  FOP_UNK   = 0,            ///< FusedOpType: unknown fused operation

  // Field manipulations
  FOP_SEXT  = 10,           ///< FusedOpType: sign extension
  FOP_ZEXT  = 11,           ///< FusedOpType: zero extension

  // Bit manipulations
  FOP_NOT   = 20,           ///< FusedOpType: bit complement
  FOP_NEG   = 21            ///< FusedOpType: negation

}FusedOpType;               ///< CoreGenSigMap: Emumerated types to represent fused micro ops

typedef enum{
  // Generic signals
  SIGUNK    = 0,            ///< SigType: Unknown generic signal
  SIGINSTF  = 1,            ///< SigType: Instruction field

  // ALU signals
  ALU_ADD   = 2,            ///< SigType: ALU Add
  ALU_SUB   = 3,            ///< SigType: ALU Subtract
  ALU_SLL   = 4,            ///< SigType: ALU Shift left logical
  ALU_SRL   = 5,            ///< SigType: ALU Shift right logical
  ALU_SRA   = 6,            ///< SigType: ALU Shift right arithmetic
  ALU_AND   = 7,            ///< SigType: ALU Logical AND
  ALU_OR    = 8,            ///< SigType: ALU Logical OR
  ALU_XOR   = 9,            ///< SigType: ALU Logical XOR
  ALU_SLT   = 10,           ///< SigType: ALU Set less than
  ALU_SLTU  = 11,           ///< SigType: ALU Set less than unsigned
  ALU_COPY  = 12,           ///< SigType: ALU Copy
  ALU_MUL   = 13,           ///< SigType: ALU Multiplier
  ALU_DIV   = 14,           ///< SigType: ALU Divide
  ALU_REM   = 15,           ///< SigType: ALU Remainder

  ALU_FADD  = 20,           ///< SigType: ALU Floating point add
  ALU_FSUB  = 21,           ///< SigType: ALU Floating point subtract
  ALU_FMUL  = 22,           ///< SigType: ALU Floating point multiply
  ALU_FDIV  = 23,           ///< SigType: ALU Floating point divide
  ALU_FREM  = 24,           ///< SigType: ALU Floating point remainder

  // PC signals
  PC_INCR   = 30,           ///< SigType: PC Increment
  PC_BRJMP  = 31,           ///< SigType: PC Branch jump target
  PC_JALR   = 32,           ///< SigType: PC Jump register target

  // Branch signals
  BR_N      = 40,           ///< SigType: Next
  BR_NE     = 41,           ///< SigType: Branch not equal
  BR_EQ     = 42,           ///< SigType: Branch equal
  BR_GT     = 43,           ///< SigType: Branch greater than
  BR_GTU    = 44,           ///< SigType: Branch greater than unsigned
  BR_GE     = 45,           ///< SigType: Branch greater than equal to signed
  BR_GEU    = 46,           ///< SigType: Branch greater than equal to unsigned
  BR_LT     = 47,           ///< SigType: Branch less than signed
  BR_LTU    = 48,           ///< SigType: Branch less than unsigned
  BR_LE     = 49,           ///< SigType: Branch less than equal to signed
  BR_LEU    = 50,           ///< SigType: Branch less than equal to unsigned
  BR_J      = 51,           ///< SigType: Jump
  BR_JR     = 52,           ///< SigType: Jump Register

  // Mux signals
  MUX_NE    = 60,           ///< SigType: Compare not equal
  MUX_EQ    = 61,           ///< SigType: Compare equal
  MUX_GT    = 62,           ///< SigType: Compare greater than
  MUX_GTU   = 63,           ///< SigType: Compare greater than unsigned
  MUX_GE    = 64,           ///< SigType: Compare greater than equal to signed
  MUX_GEU   = 65,           ///< SigType: Compare greater than equal to unsigned
  MUX_LT    = 66,           ///< SigType: Compare less than
  MUX_LTU   = 67,           ///< SigType: Compare less than unsigned
  MUX_LE    = 68,           ///< SigType: Compare less than equal to signed
  MUX_LEU   = 69,           ///< SigType: Compare less than equal to unsigned
  MUX       = 70,           ///< SigType: Generic Mux enable signal

  // Register signals
  REG_READ  = 80,           ///< SigType: Register read
  REG_WRITE = 81,           ///< SigType: Register write
  AREG_READ = 82,           ///< SigType: ALU register read
  AREG_WRITE= 83,           ///< SigType: ALU register write

  // Memory signals
  MEM_READ  = 90,           ///< SigType: Memory read
  MEM_WRITE = 91,           ///< SigType: Memory write

  // Misc signals
  FENCE     = 100           ///< SigType: Fence
}SigType;                   ///< CoreGenSigMap: Emumerated types to represent signals

class SCSig{
private:
  SigType Type;             ///< SCSig: Signal type
  FusedOpType FType;        ///< SCSig: Fused of type
  unsigned SigWidth;        ///< SCSig: Width of the target signal (in bits)
  signed DistanceTrue;      ///< SCSig: Branch distance (in uOps) for default branches
  signed DistanceFalse;     ///< SCSig: Branch distance (in uOps) for alternate targets
  std::string Inst;         ///< SCSig: Name of the instruction
  std::string Name;         ///< SCSig: Name of the signal
  std::string PipeName;     ///< SCSIg: Name of the respective pipeline stage
  std::vector<std::string> Inputs; ///< SCSig: vector of required inputs for ALU ops

public:
  /// Default constructor
  SCSig(SigType T);

  /// Overloaded constructor
  SCSig(SigType T, unsigned W);

  /// Overloaded constructor
  SCSig(SigType T, unsigned W, std::string I, std::string P);

  /// Overload constructor
  SCSig(SigType T, std::string I);

  /// Overloaded constructor
  SCSig(SigType T, std::string I, std::string N);

  /// Overloaded constructor
  SCSig(SigType T, unsigned W, signed DT, signed DF, std::string I, std::string P);

  /// Overloaded constructor
  SCSig(SigType T, unsigned W, std::string I, std::string N, std::string P);

  /// Overloaded constructor
  SCSig(SigType T, unsigned W, signed DT, signed DF, std::string I, std::string N, std::string P );

  /// Default destructor
  ~SCSig();

  /// Converts the signal name to a std::string
  const std::string SigTypeToStr();

  /// Converts the fused op type to a std::string
  const std::string FusedOpTypeToStr();

  /// Returns true if the target signal is an ALU signal
  bool isALUSig();

  /// Returns true if the target signal is a MUX signal
  bool isMuxSig();

  /// Returns true if the target signal is a PC branch
  bool isPCSig();

  /// Returns true if the target signal is a memory signal
  bool isMemSig();

  /// Returns true if the target signal is a register signal
  bool isRegSig();

  /// Returns true if the target signal is a branch signal
  bool isBranchSig();

  /// Returns the number of required input operands found for the target signal
  unsigned GetNumInputs() { return Inputs.size(); }

  /// Inserts a new required input operand into the target signal
  bool InsertInput( std::string Input );

  /// Retrieves the input at the desired index
  std::string GetInput( unsigned Idx );

  /// Sets the signal type
  bool SetType( SigType Type );

  /// Sets the fused operation type
  bool SetFusedType( FusedOpType Type );

  /// Sets the branch distance for default branches (in uOps)
  bool SetDistanceTrue(signed D);

  /// Sets the branch distance for alternate branches (in uOps)
  bool SetDistanceFalse(signed D);

  /// Retrieves the distance for default branches (in uOps)
  signed GetDistanceTrue() { return DistanceTrue; }

  /// Retrieves the distance for alternate branches (in uOps)
  signed GetDistanceFalse() { return DistanceFalse; }

  /// Retrieves the signal type
  SigType GetType() { return Type; }

  /// Retrieves the fused type
  FusedOpType GetFusedType() { return FType; }

  /// Sets the width of the target signal
  bool SetWidth( unsigned Width );

  /// Retrieves the width of the target signal
  unsigned GetWidth() { return SigWidth; }

  /// Sets the signal name
  bool SetName( std::string Name );

  /// Determines whether there is a specific pipe stage defined
  bool IsPipeDefined();

  /// Retrieves the pipeline name
  std::string GetPipeName() { return PipeName; }

  /// Sets the pipeline name
  bool SetPipeName( std::string PName );

  /// Retrieves the signal name
  std::string GetName() { return Name; }

  /// Set the instruction name for the signal
  bool SetInst( std::string Inst );

  /// Retrieves the instruction name for the signal
  std::string GetInst() { return Inst; }
};

#endif

// EOF
