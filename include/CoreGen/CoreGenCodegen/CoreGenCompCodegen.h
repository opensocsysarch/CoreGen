//
// _CoreGenCompCodegen_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenCompCodegen
 *
 * \ingroup CoreGenCodegen
 *
 * \brief The CoreGen compiler code generation class
 *
 */

#ifndef _COREGENCOMPCODEGEN_H_
#define _COREGENCOMPCODEGEN_H_

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
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenArchive/CoreGenArchive.h"

// Codegen Headers
#include "CoreGen/CoreGenCodegen/CoreGenLLVMCodegen.h"
#include "CoreGen/CoreGenCodegen/CoreGenLLVMCodegens.h"

class CoreGenCompCodegen
{
private:
  CoreGenNode *Top;                 ///< Top-level coregen node
  CoreGenProj *Proj;                ///< CoreGen Project Info
  CoreGenArchEntry *Entry;          ///< CoreGenArchEntry for the target Comp version
  std::string LLVMRoot;             ///< Root directory for Comp output
  CoreGenErrno *Errno;              ///< CoreGen Errno Structure

public:
  /// Default constructor
  CoreGenCompCodegen(CoreGenNode *T,
                     CoreGenProj *P,
                     CoreGenArchEntry *EN,
                     std::string R,
                     CoreGenErrno *E);

  /// Default destructor
  ~CoreGenCompCodegen();

  /// Execute the compiler codegen
  bool Execute();

};

#endif

// EOF
