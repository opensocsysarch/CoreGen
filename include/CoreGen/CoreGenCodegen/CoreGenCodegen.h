//
// _CoreGenCodegen_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/** \defgroup CoreGenCodegen CoreGen High Level System on Chip (SoC) Design Framework Code Generator
 *
 * \brief The CoreGenCodegen library provides code generation facilities for the CoreGen infrastructure
 *
 */


/**
 * \class CoreGenCodegen
 *
 * \ingroup CoreGenCodegen
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
#include <vector>

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenArchive/CoreGenArchive.h"

// Codegen Headers
#include "CoreGen/CoreGenCodegen/CoreGenChiselCodegen.h"
#include "CoreGen/CoreGenCodegen/CoreGenCompCodegen.h"


class CoreGenCodegen
{
private:
  CoreGenNode *Top;           ///< Top-level coregen node
  CoreGenProj *Proj;          ///< CoreGen Project Info
  CoreGenEnv *Env;            ///< CoreGen environment
  CoreGenErrno *Errno;        ///< CoreGen Errno Structure
  CoreGenArchive *Archive;    ///< CoreGen Archive Object
  CoreGenArchEntry *LLEntry;  ///< CoreGen LLVM archive entry
  CoreGenPluginMgr* PlugInMgr; ///< CoreGen PlugIn Manager

  std::string ChiselDir;      ///< CoreGen Chisel output directory
  std::string LLVMDir;        ///< CoreGen LLVM output directory
  std::string SCDir;          ///< CoreGen StoneCutter output directory

  std::vector<std::string> InstFormatsVect; ///< CoreGen instruction format vector across insts

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

  /// Builds the Chisel src output
  bool BuildChisel();

  /// Builds the compiler directory structure
  bool BuildLLVMDir();

  /// Initializes the LLVM source
  bool InitLLVMSrc(std::string CompVer);

  /// Builds the stonecutter dierctory structure
  bool BuildStoneCutterDir();

  /// Constructs full StoneCutter files for each ISA
  bool BuildStoneCutterFiles();

  /// Constructs an individual ISA Chisel File
  bool BuildISAChisel(CoreGenISA *ISA,
                      std::vector<CoreGenInst *> Insts);

  /// Constructs the common package block
  bool BuildChiselCommonPackage();

  /// Init the internal StoneCutter intrinsics
  void InitIntrinsics();

  /// Retrieve a StoneCutter syntax string of register attributes
  std::string GetRegAttrStr(CoreGenReg *Reg);

public:
  /// Default constructor
  CoreGenCodegen(CoreGenNode *T, CoreGenProj *P, CoreGenEnv *V, CoreGenErrno *E, CoreGenPluginMgr *PlugIn);

  /// Default destructor
  ~CoreGenCodegen();

  /// Execute the LLVM codegen
  bool ExecuteLLVMCodegen(std::string CompVer);

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
