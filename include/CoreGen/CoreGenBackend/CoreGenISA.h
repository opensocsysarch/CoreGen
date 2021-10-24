//
// _CoreGenISA_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenISA
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Instruction Set Handler
 *
 */

#ifndef _COREGENISA_H_
#define _COREGENISA_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

class CoreGenISA : public CoreGenNode{
private:
public:
  /// Default Constructor
  CoreGenISA(CoreGenErrno *Errno);

  /// Overloaded Constructor
  CoreGenISA( std::string Name, CoreGenErrno *Errno );

  /// Default Destructor
  ~CoreGenISA();
};

#endif

// EOF
