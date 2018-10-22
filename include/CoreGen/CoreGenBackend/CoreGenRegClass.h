//
// _CoreGenRegClass_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenRegClass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Register Class Node Handler
 *
 */

#ifndef _COREGENREGCLASS_H_
#define _COREGENREGCLASS_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenReg.h"

class CoreGenRegClass : public CoreGenNode{
private:

public:
  /// Default Constructor
  CoreGenRegClass(CoreGenErrno *E);

  /// Overloaded Constructor
  CoreGenRegClass(std::string Name, CoreGenErrno *E);

  /// Retrieve the number of child registers
  unsigned GetNumReg() { return GetNumChild(); }

  /// Add a register to the register class
  bool InsertReg( CoreGenReg *R );

  /// Retrieve a target register
  CoreGenReg *GetReg(unsigned R);

  /// Default Destructor
  ~CoreGenRegClass();
};

#endif

// EOF
