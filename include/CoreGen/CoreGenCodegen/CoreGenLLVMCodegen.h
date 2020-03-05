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
#include <cmath>
#include <locale>

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenNodes.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenArchive/CoreGenArchive.h"

class CoreGenLLVMCodegen {
protected:
  CoreGenNode *Top;                 ///< Top-level coregen node
  CoreGenProj *Proj;                ///< CoreGen Project Info
  CoreGenArchEntry *Entry;          ///< CoreGenArchEntry for the target LLVM version
  std::string LLVMRoot;             ///< Root directory for LLVM output
  CoreGenErrno *Errno;              ///< CoreGen Errno Structure
  std::string Version;              ///< LLVM Version Number

  std::string TargetName;           ///< Name of the compiler target

  std::vector<std::string> Subtargets;        ///< vector of subtarget ISAs
  std::vector<CoreGenInstFormat *> Formats;   ///< vector of instruction formats
  std::vector<CoreGenRegClass *> RegClasses;  ///< vector of register classes
  std::vector<CoreGenInst *> Insts;           ///< vector of instructions
  std::vector<CoreGenPseudoInst *> PInsts;    ///< vector of pseudo instructions

  /// Generate the vector of subtarget nodes
  bool GenerateSubtargets();

  /// Generate the vector of instruction format nodes
  bool GenerateInstFormats();

  /// Generate the vector of register class nodes
  bool GenerateRegClasses();

  /// Generate the vector of instruction nodes
  bool GenerateInsts();

  /// Generates the vector of pseudo instruction nodes
  bool GeneratePInsts();

  /// Convert a string to upper case
  std::string UpperCase(std::string Str);

public:
  /// Default constructor
  CoreGenLLVMCodegen(CoreGenNode *T,
                     CoreGenProj *P,
                     CoreGenArchEntry *EN,
                     std::string R,
                     CoreGenErrno *E,
                     std::string Ver);

  /// Default destructor
  virtual ~CoreGenLLVMCodegen();

  /// Execute the LLVM codegen
  virtual bool Execute() { return false; }

  /// Execute the backend LLVM codegen
  bool ExecuteBackend();

  /// Retrieve the version info
  std::string GetVersion() { return Version; }

};

#endif

// EOF
