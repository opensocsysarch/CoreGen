//
// _CoreGenCodegen_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenCodegen
 *
 * \ingroup Coregen
 *
 * \brief The CoreGen code generation base class
 *
 */

#ifndef _COREGENCODEGEN_H_
#define _COREGENCODEGEN_H_

// Standard Headers
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

class CoreGenCodegen
{
private:
  CoreGenNode *Top;           ///< Top-level coregen node
  CoreGenProj *Proj;          ///< CoreGen Project Info
  CoreGenErrno *Errno;        ///< CoreGen Errno Structure

  std::string ChiselDir;      ///< CoreGen Chisel output directory
  std::string LLVMDir;        ///< CoreGen LLVM output directory
  std::string SCDir;          ///< CoreGen StoneCutter output directory

  bool isTopMakefile;         ///< Has the top-level makefile been constructed?

  /// Builds the top-level project makefile
  bool BuildProjMakefile();

  /// Builds the chisel directory structure
  bool BuildChiselDir();

  /// Builds the Chisel makefile
  bool BuildChiselMakefile();

  /// Builds the Chisel SBT file
  bool BuildChiselSBT();

  /// Builds the Chisel project files
  bool BuildChiselProject();

  /// Builds the compiler directory structure
  bool BuildLLVMDir();

  /// Builds the stonecutter dierctory structure
  bool BuildStoneCutterDir();

  /// Init the internal StoneCutter intrinsics
  void InitIntrinsics();

public:
  /// Default constructor
  CoreGenCodegen(CoreGenNode *T, CoreGenProj *P, CoreGenErrno *E);

  /// Default destructor
  ~CoreGenCodegen();

  /// Execute the LLVM codegen
  bool ExecuteLLVMCodegen();

  /// Execute the Chisel codegen
  bool ExecuteChiselCodegen();

  /// Execute the Chisel and LLVM codegens
  bool Execute();

  /// Retrieve the chisel output directory
  std::string GetChiselDir() { return ChiselDir; }

  /// Retrieve the LLVM output directory
  std::string GetLLVMDir() { return LLVMDir; }

  /// Retrieve the StoneCutter output directory
  std::string GetStoneCutterDir() { return SCDir; }
};

#endif

// EOF
