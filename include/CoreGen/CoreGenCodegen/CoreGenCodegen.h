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

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNodes.h"

class CoreGenCodegen
{
private:
  std::string BaseDir;
  CoreGenErrno *Errno;

public:
  /// Default constructor
  CoreGenCodegen(std::string Base, CoreGenErrno *E);

  /// Default destructor
  ~CoreGenCodegen();

  /// Execute the codegen
  bool Execute();
};

#endif

// EOF
