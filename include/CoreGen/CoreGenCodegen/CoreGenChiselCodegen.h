//
// _CoreGenChiselCodegen_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenChiselCodegen
 *
 * \ingroup CoreGenCodegen
 *
 * \brief The CoreGen Chisel code generation class
 *
 */

#ifndef _COREGENCHISELCODEGEN_H_
#define _COREGENCHISELCODEGEN_H_

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

class CoreGenChiselCodegen
{
private:
  CoreGenNode *Top;                 ///< Top-level coregen node
  CoreGenProj *Proj;                ///< CoreGen Project Info
  std::string ChiselRoot;           ///< Root directory for chisel output
  CoreGenErrno *Errno;              ///< CoreGen Errno Structure


public:
  /// Default constructor
  CoreGenChiselCodegen(CoreGenNode *T,
                       CoreGenProj *P,
                       std::string R,
                       CoreGenErrno *E);

  /// Default destructor
  ~CoreGenChiselCodegen();

  /// Execute the Chisel codegen
  bool Execute();

};

#endif

// EOF
