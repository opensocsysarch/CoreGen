//
// _SCChiselCodeGen_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCChiselCodeGen
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Chisel code generation class
 *
 */

#ifndef _STONECUTTER_SCCHISELCODEGEN_H_
#define _STONECUTTER_SCCHISELCODEGEN_H_

// standard headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <locale>
#include <regex>

// llvm headers
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

// coregen headers
#include "CoreGen/StoneCutter/SCParser.h"
#include "CoreGen/StoneCutter/SCOpts.h"
#include "CoreGen/StoneCutter/SCMsg.h"
#include "CoreGen/StoneCutter/SCUtil.h"
#include "CoreGen/StoneCutter/SCIntrinsics.h"
#include "CoreGen/StoneCutter/SCPass.h"
#include "CoreGen/StoneCutter/SCPasses.h"
#include "CoreGen/StoneCutter/SCPipeInfo.h"
#include "CoreGen/StoneCutter/SCPerf.h"

#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

using namespace llvm;

class SCChiselCodeGen{
private:
  SCParser *Parser;                           ///< SC Parser object
  SCOpts *Opts;                               ///< SC Options object
  SCMsg *Msgs;                                ///< SC Messages object
  SCPerf *Perf;                               ///< SC Perf object
  std::string ChiselFile;                     ///< Output file
  std::string SigMap;                         ///< Signal map output file

  CoreGenSigMap *CSM;                         ///< Signal map

  std::ofstream OutFile;                      ///< Output file stream

  std::vector<SCIntrin *> Intrins;            ///< StoneCutter Intrinsics
  std::vector<SCPass *> Passes;               ///< StoneCutter CodeGen passes

  // private functions
  bool ExecuteCodegen();                          ///< Generates chisel from the LLVM IR
  void InitIntrinsics();                          ///< Init the intrinsics vector
  void InitPasses();                              ///< Init the pass vector
  bool ExecutePasses();                           ///< Executes all the code generation passes
  bool ExecuteSignalMap();                        ///< Executes the signal map generator
  bool ExecuteRISCPipelineOpt(CoreGenSigMap *SM); ///< Executes the standard RISC pipeliner
  bool ExecuteVLIWPipelineOpt(CoreGenSigMap *SM); ///< Executes the standard RISC pipeliner
  bool ExecutePipelineOpt(CoreGenSigMap *SM);     ///< Executes the pipeline optimizer
  bool ExecuteUcodeCodegen();                     ///< Executes a microcode codegen using sigmaps
  bool ExecuteManualCodegen();                    ///< Executes a manual codegen
  bool ExecuteVLIWCodegen();                      ///< Executes a vliw microcode codegen using sigmaps
  bool ExecuteVLIWUcodeCodegen(CoreGenSigMap *SM,
                               SCPipeInfo *Pinfo,
                               std::string Stage);///< Executes a vliw microcode table builder for the target pipeline

  void WriteChiselHeader(std::ofstream &out, std::string FName ); ///< Writes a header to the chisel output file
  bool DeriveBitPat(std::string &BP);         ///< Derives the bit pattern for the microcode generator
  bool WriteUCodeCompiler();                  ///< Writes the microcode compiler

  void WriteUCodeTableComment(SCPipeInfo *P); ///< Writes the microcode table comment to the Chisel file
  void WriteVLIWUCodeTableComment(std::ofstream &LOutFile,
                                  SCPipeInfo *P); ///< Writes the microcode table comment to the Chisel file

  void WriteFETCHUOp(SCPipeInfo *PInfo);      ///< Writes the Fetch micro op
  void WriteNOPUOp(SCPipeInfo *PInfo);        ///< Writes the NOP micro op
  void WriteILLEGALUOp(SCPipeInfo *PInfo);    ///< Writes the ILLEGAL micro op
  void WriteUNIMPLUOp(SCPipeInfo *PInfo);     ///< Writes the UNIMPL micro op
  void WriteINITPCUOp(SCPipeInfo *PInfo);     ///< Writes the INITPC micro op
  void WriteUOp(SCSig *Sig, unsigned NumSigs,
                unsigned NSig, SCPipeInfo *PInfo);  ///< Write all the micro ops

  void WriteVLIWFETCHUOp(std::ofstream &LOutFile,
                         SCPipeInfo *PInfo);      ///< Writes the VLIW Fetch micro op
  void WriteVLIWNOPUOp(std::ofstream &LOutFIle,
                       SCPipeInfo *PInfo);        ///< Writes the VLIW NOP micro op
  void WriteVLIWILLEGALUOp(std::ofstream &LOutFile,
                           SCPipeInfo *PInfo);    ///< Writes the VLIW ILLEGAL micro op
  void WriteVLIWUNIMPLUOp(std::ofstream &LOutFile,
                          SCPipeInfo *PInfo);     ///< Writes the VLIW UNIMPL micro op
  void WriteVLIWINITPCUOp(std::ofstream &LOutFile,
                          SCPipeInfo *PInfo);     ///< Writes the VLIW INITPC micro op
  void WriteVLIWUOp(std::ofstream &LOutFile,
                    SCSig *Sig, unsigned NumSigs,
                    unsigned NSig, SCPipeInfo *PInfo);  ///< Write all the VLIW micro ops

  inline bool IsFinalUOp( unsigned NumSigs, unsigned NSig );  ///< Determines if this is the final micro op

  std::string DecodeRegSlot(std::string &SigStr,
                            SCPipeInfo *PInfo,
                            bool &IsImm);                     ///< Decodes the requested signal to the register slot
  std::string SCToUpper(std::string Str);                     ///< Capitalizes the requested string
  bool IsInteger(std::string Str);                            ///< Determines if the target string is an integer constant
  bool IsRegEnabled(SCSig *Sig, std::string Field);           ///< Determines if the target Field is enabled as an input


  ///< Emit the NOP uOP
  void EmitNOP(std::ofstream &LOutFile,
               SCSig *Sig,
               unsigned NumSigs,
               unsigned NSig,
               std::string &BrTarget,
               std::string &UOpBr,
               SCPipeInfo *PInfo );

  ///< Emit the SIGINSTF uOp
  void EmitSIGINSTF(std::ofstream &LOutFile,
                    SCSig *Sig,
                    unsigned NumSigs,
                    unsigned NSig,
                    std::string &BrTarget,
                    std::string &UOpBr,
                    SCPipeInfo *PInfo );

  ///< Emit the ALU uOP's
  void EmitALU_OP(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the SLT uOp
  void EmitALU_SLT(std::ofstream &LOutFile,
                   SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the SLTU uOp
  void EmitALU_SLTU(std::ofstream &LOutFile,
                    SCSig *Sig,
                    unsigned NumSigs,
                    unsigned NSig,
                    std::string &BrTarget,
                    std::string &UOpBr,
                    SCPipeInfo *PInfo );

  ///< Emit the COPY uOp
  void EmitALU_COPY(std::ofstream &LOutFile,
                    SCSig *Sig,
                    unsigned NumSigs,
                    unsigned NSig,
                    std::string &BrTarget,
                    std::string &UOpBr,
                    SCPipeInfo *PInfo );

  ///< Emit the PC_INCR uOp
  void EmitPC_INCR(std::ofstream &LOutFile,
                   SCSig *Sig,
                   unsigned NumSigs,
                   unsigned NSig,
                   std::string &BrTarget,
                   std::string &UOpBr,
                   SCPipeInfo *PInfo );

  ///< Emit the PC_BRJMP uOp
  void EmitPC_BRJMP(std::ofstream &LOutFile,
                    SCSig *Sig,
                    unsigned NumSigs,
                    unsigned NSig,
                    std::string &BrTarget,
                    std::string &UOpBr,
                    SCPipeInfo *PInfo );

  ///< Emit the PC_JALR uOp
  void EmitPC_JALR(std::ofstream &LOutFile,
                   SCSig *Sig,
                   unsigned NumSigs,
                   unsigned NSig,
                   std::string &BrTarget,
                   std::string &UOpBr,
                   SCPipeInfo *PInfo );

  ///< Emit the BR_N uOp
  void EmitBR_N(std::ofstream &LOutFile,
                SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );

  ///< Emit the BR_NE uOp
  void EmitBR_NE(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &AltTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

  ///< Emit the BR_EQ uOp
  void EmitBR_EQ(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &AltTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

  ///< Emit the BR_GT uOp
  void EmitBR_GT(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &AltTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

  ///< Emit thte BR_GTU uOp
  void EmitBR_GTU(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &AltTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the BR_GE uOp
  void EmitBR_GE(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &AltTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

  ///< Emit the BR_GEU uOp
  void EmitBR_GEU(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &AltTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the BR_LT uOp
  void EmitBR_LT(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &AltTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

  ///< Emit the BR_LTU uOp
  void EmitBR_LTU(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &AltTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the BR_LE uOp
  void EmitBR_LE(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &AltTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

  ///< Emit the BR_LEU uOp
  void EmitBR_LEU(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &AltTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the BR_J uOp
  void EmitBR_J(std::ofstream &LOutFile,
                SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );

  ///< Emit the BR_JR uOp
  void EmitBR_JR(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

  ///< Emit the MUX_NE uOp
  void EmitMUX_NE(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the MUX_EQ uOp
  void EmitMUX_EQ(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the MUX_GT uOp
  void EmitMUX_GT(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the MUX_GTU uOp
  void EmitMUX_GTU(std::ofstream &LOutFile,
                   SCSig *Sig,
                   unsigned NumSigs,
                   unsigned NSig,
                   std::string &BrTarget,
                   std::string &UOpBr,
                   SCPipeInfo *PInfo );

  ///< Emit the MUX_GE uOp
  void EmitMUX_GE(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the MUX_GEU uOp
  void EmitMUX_GEU(std::ofstream &LOutFile,
                   SCSig *Sig,
                   unsigned NumSigs,
                   unsigned NSig,
                   std::string &BrTarget,
                   std::string &UOpBr,
                   SCPipeInfo *PInfo );

  ///< Emit the MUX_LT uOp
  void EmitMUX_LT(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the MUX_LTU uOp
  void EmitMUX_LTU(std::ofstream &LOutFile,
                   SCSig *Sig,
                   unsigned NumSigs,
                   unsigned NSig,
                   std::string &BrTarget,
                   std::string &UOpBr,
                   SCPipeInfo *PInfo );

  ///< Emit the MUX_LE uOp
  void EmitMUX_LE(std::ofstream &LOutFile,
                  SCSig *Sig,
                  unsigned NumSigs,
                  unsigned NSig,
                  std::string &BrTarget,
                  std::string &UOpBr,
                  SCPipeInfo *PInfo );

  ///< Emit the MUX_LEU uOp
  void EmitMUX_LEU(std::ofstream &LOutFile,
                   SCSig *Sig,
                   unsigned NumSigs,
                   unsigned NSig,
                   std::string &BrTarget,
                   std::string &UOpBr,
                   SCPipeInfo *PInfo );

  ///< Emit the MUX uOp
  void EmitMUX(std::ofstream &LOutFile,
               SCSig *Sig,
               unsigned NumSigs,
               unsigned NSig,
               std::string &BrTarget,
               std::string &UOpBr,
               SCPipeInfo *PInfo );

  ///< Emit the REG_READ uOp
  void EmitREG_READ(std::ofstream &LOutFile,
                    SCSig *Sig,
                    unsigned NumSigs,
                    unsigned NSig,
                    std::string &BrTarget,
                    std::string &UOpBr,
                    SCPipeInfo *PInfo );

  ///< Emit the REG_WRITE uOp
  void EmitREG_WRITE(std::ofstream &LOutFile,
                     SCSig *Sig,
                     unsigned NumSigs,
                     unsigned NSig,
                     std::string &BrTarget,
                     std::string &UOpBr,
                     SCPipeInfo *PInfo );

  ///< Emit the AREG_READ uOp
  void EmitAREG_READ(std::ofstream &LOutFile,
                     SCSig *Sig,
                     unsigned NumSigs,
                     unsigned NSig,
                     std::string &BrTarget,
                     std::string &UOpBr,
                     SCPipeInfo *PInfo );

  ///< Emit the AREG_WRITE uOp
  void EmitAREG_WRITE(std::ofstream &LOutFile,
                      SCSig *Sig,
                      unsigned NumSigs,
                      unsigned NSig,
                      std::string &BrTarget,
                      std::string &UOpBr,
                      SCPipeInfo *PInfo );

  ///< Emit the MEM_READ uOp
  void EmitMEM_READ(std::ofstream &LOutFile,
                    SCSig *Sig,
                    unsigned NumSigs,
                    unsigned NSig,
                    std::string &BrTarget,
                    std::string &UOpBr,
                    SCPipeInfo *PInfo );

  ///< Emit the MEM_WRITE uOp
  void EmitMEM_WRITE(std::ofstream &LOutFile,
                     SCSig *Sig,
                     unsigned NumSigs,
                     unsigned NSig,
                     std::string &BrTarget,
                     std::string &UOpBr,
                     SCPipeInfo *PInfo );

  ///< Emit the FENCE uOp
  void EmitFENCE(std::ofstream &LOutFile,
                 SCSig *Sig,
                 unsigned NumSigs,
                 unsigned NSig,
                 std::string &BrTarget,
                 std::string &UOpBr,
                 SCPipeInfo *PInfo );

public:

  /// Default constructor
  SCChiselCodeGen(SCParser *, SCOpts *, SCMsg *, std::string);

  /// Default desctrutor
  ~SCChiselCodeGen();

  /// Generate the LLVM Chisel output
  bool GenerateChisel();

  /// Generate the signal map using the StoneCutter input
  bool GenerateSignalMap(std::string);

  /// Retrieves a vector of the Chisel codegen passes
  std::vector<std::string> GetPassList();

  /// Retrieves a vector of the Chisel codegen pass options
  std::vector<std::string> GetOptsList();
};

#endif

// EOF
