//
// _LLVM801CG_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//


/**
 * \class LLVM801CG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenLLVMCodegen derived class for LLVM Version 8.0.1
 *
 */

#ifndef _LLVM801CG_H_
#define _LLVM801CG_H_

#include "CoreGen/CoreGenCodegen/CoreGenLLVMCodegen.h"

class LLVM801CG  : public CoreGenLLVMCodegen {
private:

  /// Generate the directory structure for the new target
  bool GenerateTargetDir();

  /// Generate the target implementation for the new target
  bool GenerateTargetImpl();

  /// Generate the target CPU drive
  bool GenerateCPUDriver();

  /// Generate the build infrastructure
  bool GenerateBuildImpl();

  // TargetImpl Drivers

  /// Generate a string for the target pseudo instruction
  std::string TIDerivePseudoInstArgs(CoreGenPseudoInst *PInst);

  /// Generate a vector of tuples that represent the unique immediate fields
  std::map<std::string,unsigned> TIGenerateImmFieldVector();

  /// Generate the instruction input args using the instruction fields
  std::string TIGenerateInstFields(CoreGenInstFormat *Format);

  /// Generate the instruction format input args using the instruction fields
  std::string TIGenerateInstArgsFields(CoreGenInstFormat *Format);

  /// Generate the instruction argument list with the associated register classes
  std::string TIGenerateRegClassImmList(CoreGenInstFormat *Format);

  /// Generate the instruction argument list without the associated register classes
  std::string TIGenerateRegImmList(CoreGenInstFormat *Format);

  /// Generate the number of bits required to store the instruction format
  unsigned TIGenerateInstFormatBits(unsigned NumFormats);

  /// Generate the top-level TableGen file
  bool TIGenerateTopLevelTablegen();

  /// Generate the individual instructon type tablegen files
  bool TIGenerateISATablegen();

  /// Generate the register tablegen
  bool TIGenerateRegisterTablegen();

  /// Generate the individual instruction tablegen files
  bool TIGenerateInstTablegen();

  /// Generate the system tablegen
  bool TIGenerateSystemTablegen();

  /// Generate the calling convention tablegen
  bool TIGenerateCallingConvTablegen();

  /// Generate the TableGen info
  bool TIGenerateTablegen();

  /// Generate the ISel Dag info
  bool TIGenerateISelDag();

  /// Generate the intruction lowering
  bool TIGenerateInstLowering();

  /// Generate the register info template
  bool TIGenerateRegInfo();

  /// Generate the subtarget info
  bool TIGenerateSubtargetInfo();

  /// Generate the target machine template
  bool TIGenerateTargetMach();

  /// Generate the target object file template
  bool TIGenerateTargetObj();

  /// Generate the target asm parser
  bool TIGenerateAsmParser();

  /// Generate the target disassembler
  bool TIGenerateDisass();

  /// Generate the target instruction printer
  bool TIGenerateInstPrinter();

  /// Generate the target MCTargetDesc
  bool TIGenerateMCTargetDesc();

  /// Generate the TargetInfo
  bool TIGenerateTargetInfo();

  /// Generate the top-level header
  bool TIGenerateTargetHeader();

  /// Generate the top-level CMake scripts
  bool TIGenerateCmake();

  // Generate the MCTarget build infrastructure
  bool TIGenerateMCBuild();

  // Generate the MCTarget AsmBackend
  bool TIGenerateMCAsmBackend();

  // Generate the MCTarget Elf Object Writer
  bool TIGenerateMCElfObjWriter();

  // Generate the MCTarget ElfStreamer
  bool TIGenerateMCElfStreamer();

  // Generate the MCTarget FixupKinds
  bool TIGenerateMCFixupKinds();

  // Generate the MCTarget InstPrinter
  bool TIGenerateMCInstPrinter();

  // Generate the MCTarget Asm Info
  bool TIGenerateMCAsmInfo();

  // Generate the MCTarget CodeEmitter
  bool TIGenerateMCCodeEmitter();

  // Generate the MCTarget Expression Handler
  bool TIGenerateMCExpr();

  // Generate the MCTarget Target Description
  bool TIGenerateMCTargetDescCore();

  // Generate the MCTarget Target Streamer
  bool TIGenerateMCTargetStreamer();

public:
  /// Default constructor
  LLVM801CG(CoreGenNode *T,
            CoreGenProj *P,
            CoreGenArchEntry *EN,
            std::string R,
            CoreGenErrno *E);

  /// Default destructor
  ~LLVM801CG();

  /// Execute the LLVM codegen
  virtual bool Execute();

};

#endif

// EOF
