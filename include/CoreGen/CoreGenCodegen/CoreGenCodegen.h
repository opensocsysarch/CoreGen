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
  std::string BaseDir;        ///< Base directory to generate all output files
  CoreGenErrno *Errno;        ///< CoreGen Errno Structure

  std::string FullDir;        ///< Fully qualified directory structure

  /// Builds the chisel directory structure
  bool BuildChiselDir();      ///< Constructs the Chisel directory structure

  /// Builds the chisel makefile
  bool BuildChiselMakefile();

public:
  /// Default constructor
  CoreGenCodegen(CoreGenNode *T, std::string Base, CoreGenErrno *E);

  /// Default destructor
  ~CoreGenCodegen();

  /// Execute the LLVM codegen
  bool ExecuteLLVMCodegen();

  /// Execute the Chisel codegen
  bool ExecuteChiselCodegen();

  /// Execute all the codegens
  bool Execute();
};

#endif

// EOF
