//
// _SCSigMap_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCSigMap
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Generates a signal map for the target ISA
 *
 */

#ifndef _STONECUTTER_SCPASS_SCSIGMAP_H_
#define _STONECUTTER_SCPASS_SCSIGMAP_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCSigMap : public SCPass {
private:
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
    BR_GE     = 43,           ///< SigType: Branch greater than equal to signed
    BR_GEU    = 44,           ///< SigType: Branch greater than equal to unsigned
    BR_LT     = 45,           ///< SigType: Branch less than signed
    BR_LTU    = 46,           ///< SigType: Branch less than unsigned
    BR_J      = 47,           ///< SigType: Jump
    BR_JR     = 48,           ///< SigType: Jump Register

    // Register signals
    REG_READ  = 49,           ///< SigType: Register read
    REG_WRITE = 50,           ///< SigType: Register write

    // Memory signals
    MEM_READ  = 60,           ///< SigType: Memory read
    MEM_WRITE = 61            ///< SigType: Memory write
  }SigType;                   ///< SCSigMap: Emumerated types to represent signals

  /**
   * \class SCSig
   *
   * \ingroup StoneCutter
   *
   * \brief StoneCutter Signal: Individual signal instance
   *
   */
  class SCSig{
  private:
    SigType Type;             ///< SCSig: Signal type
    unsigned SigWidth;        ///< SCSig: Width of the target signal (in bits)
    std::string Inst;         ///< SCSig: Name of the instruction
    std::string Name;         ///< SCSig: Name of the signal

  public:
    /// Default constructor
    SCSig(SigType T);

    /// Overloaded constructor
    SCSig(SigType T, unsigned W);

    /// Overloaded constructor
    SCSig(SigType T, unsigned W, std::string I);

    /// Overload constructor
    SCSig(SigType T, std::string I);

    /// Overloaded constructor
    SCSig(SigType T, std::string I, std::string N);

    /// Overloaded constructor
    SCSig(SigType T, unsigned W, std::string I, std::string N);

    /// Default destructor
    ~SCSig();

    /// Sets the signal type
    bool SetType( SigType Type );

    /// Retrieves the signal type
    SigType GetType() { return Type; }

    /// Sets the width of the target signal
    bool SetWidth( unsigned Width );

    /// Retrieves the width of the target signal
    unsigned GetWidth() { return SigWidth; }

    /// Sets the signal name
    bool SetName( std::string Name );

    /// Retrieves the signal name
    std::string GetName() { return Name; }

    /// Set the instruction name for the signal
    bool SetInst( std::string Inst );

    /// Retrieves the instruction name for the signal
    std::string GetInst() { return Inst; }
  };

  // Private variables
  std::string SigMap;               ///< Signal map output file
  std::vector<SCSig *> Signals;     ///< Signal vector

  // Private functions
  /// walks the LLVM Module object and discovers all the interior signals
  bool DiscoverSigMap();

  /// Writes the signal map out to a Yaml file
  bool WriteSigMap();

  /// Decodes an individual instruction for the signal map
  bool CheckSigReq( Function &F, Instruction &I );

  /// Returns true if the target signal is an ALU operation
  bool isALUSig(SigType T);

  /// Returns true if the target signal is a memory operation
  bool isMemSig(SigType T);

  /// Returns true if the target signal is a register operation
  bool isRegSig(SigType T);

  /// Returns true if the target signal is a branch signal
  bool isBranchSig(SigType T);

  /// Translates binary operations to signals
  bool TranslateBinaryOp( Function &F, Instruction &I, SigType Type );

  /// Translates logical operations to signals
  bool TranslateLogicalOp( Function &F, Instruction &I, SigType Type );

  /// Translates the I/O operands to signals
  bool TranslateOperands( Function &F, Instruction &I );

public:
  /// Default cosntructor
  SCSigMap(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCSigMap();

  /// Execute the codegen
  virtual bool Execute() override;

  /// Sets the signal map file name
  bool SetSignalMapFile(std::string SM);
};

#endif

// EOF
