//
// _CoreGenLLVMCodegen_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenLLVMCodegen
 *
 * \ingroup Coregen
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

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

class CoreGenLLVMCodegen
{
private:
  CoreGenNode *Top;                 ///< Top-level coregen node
  CoreGenProj *Proj;                ///< CoreGen Project Info
  std::string LLVMRoot;             ///< Root directory for chisel output
  CoreGenErrno *Errno;              ///< CoreGen Errno Structure

public:
  /// Default constructor
  CoreGenLLVMCodegen(CoreGenNode *T,
                     CoreGenProj *P,
                     std::string R,
                     CoreGenErrno *E);

  /// Default destructor
  ~CoreGenLLVMCodegen();

  /// Execute the LLVM codegen
  bool Execute();

};

#endif

// EOF
