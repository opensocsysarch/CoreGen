//
// _SCChiselCodeGen_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

using namespace llvm;

class SCChiselCodeGen{
private:
  SCParser *Parser;                           ///< SC Parser object
  SCOpts *Opts;                               ///< SC Options object
  SCMsg *Msgs;                                ///< SC Messages object
  std::string ChiselFile;                     ///< Output file
  std::string SigMap;                         ///< Signal map output file

  CoreGenSigMap *CSM;                         ///< Signal map

  std::ofstream OutFile;                      ///< Output file stream

  std::vector<SCIntrin *> Intrins;            ///< StoneCutter Intrinsics
  std::vector<SCPass *> Passes;               ///< StoneCutter CodeGen passes

  // private functions
  void WriteChiselHeader();                   ///< Writes a header to the chisel output file
  bool ExecuteCodegen();                      ///< Generates chisel from the LLVM IR
  void InitIntrinsics();                      ///< Init the intrinsics vector
  void InitPasses();                          ///< Init the pass vector
  bool ExecutePasses();                       ///< Executes all the code generation passes
  bool ExecuteSignalMap();                    ///< Executes the signal map generator
  bool ExecuteUcodeCodegen();                 ///< Executes a microcode codegen using sigmaps
  bool ExecuteManualCodegen();                ///< Executes a manual codegen

  void WriteUCodeTableComment(SCPipeInfo *P); ///< Writes the microcode table comment to the Chisel file
  void WriteFETCHUOp(SCPipeInfo *PInfo);
  void WriteNOPUOp(SCPipeInfo *PInfo);
  void WriteILLEGALUOp(SCPipeInfo *PInfo);
  void WriteUNIMPLUOp(SCPipeInfo *PInfo);
  void WriteINITPCUOp(SCPipeInfo *PInfo);
  void WriteUOp(SCSig *Sig, unsigned NumSigs, unsigned NSig, SCPipeInfo *PInfo);

  inline bool IsFinalUOp( unsigned NumSigs, unsigned NSig );

  void EmitNOP(SCSig *Sig,
               unsigned NumSigs,
               unsigned NSig,
               std::string &BrTarget,
               std::string &UOpBr,
               SCPipeInfo *PInfo );
  void EmitSIGINSTF(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_ADD(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_SUB(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_SLL(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_SRL(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_SRA(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_AND(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_OR(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_XOR(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_SLT(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_SLTU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_COPY(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_MUL(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_DIV(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_REM(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_FADD(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_FSUB(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_FMUL(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_FDIV(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitALU_FREM(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitPC_INCR(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitPC_BRJMP(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitPC_JALR(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_N(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_NE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_EQ(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_GT(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_GTU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_GE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_GEU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_LT(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_LTU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_LE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_LEU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_J(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitBR_JR(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_NE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_EQ(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_GT(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_GTU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_GE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_GEU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_LT(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_LTU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_LE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX_LEU(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMUX(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitREG_READ(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitREG_WRITE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitAREG_READ(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitAREG_WRITE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMEM_READ(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitMEM_WRITE(SCSig *Sig,
                unsigned NumSigs,
                unsigned NSig,
                std::string &BrTarget,
                std::string &UOpBr,
                SCPipeInfo *PInfo );
  void EmitFENCE(SCSig *Sig,
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
};

#endif

// EOF
