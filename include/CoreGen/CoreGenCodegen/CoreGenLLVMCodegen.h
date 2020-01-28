//
// _CoreGenLLVMCodegen_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenLLVMCodegen
 *
 * \ingroup CoreGenCodegen
 *
 * \brief The CoreGen LLVM code generation class
 *
 */

#ifndef _COREGENLLVMCODEGEN_H_
#define _COREGENLLVMCODEGEN_H_

// Standard Headers
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <vector>

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenArchive/CoreGenArchive.h"

class CoreGenLLVMCodegen
{
private:
  CoreGenNode *Top;                 ///< Top-level coregen node
  CoreGenProj *Proj;                ///< CoreGen Project Info
  CoreGenArchEntry *Entry;          ///< CoreGenArchEntry for the target LLVM version
  std::string LLVMRoot;             ///< Root directory for LLVM output
  CoreGenErrno *Errno;              ///< CoreGen Errno Structure

  std::string TargetName;           ///< Name of the compiler target

  std::vector<std::string> Subtargets;  ///< vector of subtarget ISAs

  /// Generate the vector of subtarget nodes
  bool GenerateSubtargets();

  /// Generate the directory structure for the new target
  bool GenerateTargetDir();

  /// Generate the target implementation for the new target
  bool GenerateTargetImpl();

  /// Generate the target CPU drive
  bool GenerateCPUDriver();

  /// Generate the build infrastructure
  bool GenerateBuildImpl();

  // TargetImpl Drivers

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

public:
  /// Default constructor
  CoreGenLLVMCodegen(CoreGenNode *T,
                     CoreGenProj *P,
                     CoreGenArchEntry *EN,
                     std::string R,
                     CoreGenErrno *E);

  /// Default destructor
  ~CoreGenLLVMCodegen();

  /// Execute the LLVM codegen
  bool Execute();

};

#endif

// EOF
