//
// _CoreGenRegClass_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
  unsigned ReadPorts;       ///< CoreGenRegClass: read ports
  unsigned WritePorts;      ///< CoreGenRegCLass: write ports

public:
  /// Default Constructor
  CoreGenRegClass(CoreGenErrno *E);

  /// Overloaded Constructor
  CoreGenRegClass(std::string Name, CoreGenErrno *E);

  /// Retrieve the number of child registers
  unsigned GetNumReg() { return GetNumChild(); }

  /// Retrieve the maximum width of all the child registers
  unsigned GetMaxWidth();

  /// Add a register to the register class
  bool InsertReg( CoreGenReg *R );

  /// Retrieve a target register
  CoreGenReg *GetReg(unsigned R);

  /// Retrieve the register object by its name
  CoreGenReg *GetRegByName(std::string name);

  /// Retrieve the register object by its encoded index
  CoreGenReg *GetRegByIndex(unsigned Idx);

  /// Sets the number of read ports
  bool SetReadPorts( unsigned R ) { ReadPorts = R; return true; }

  /// Sets the number of write ports
  bool SetWritePorts( unsigned W ) { WritePorts = W; return true; }

  /// Retrieves the number of read ports
  unsigned GetReadPorts() { return ReadPorts; }

  /// Retreives the number of write ports
  unsigned GetWritePorts() { return WritePorts; }

  /// Default Destructor
  ~CoreGenRegClass();
};

#endif

// EOF
