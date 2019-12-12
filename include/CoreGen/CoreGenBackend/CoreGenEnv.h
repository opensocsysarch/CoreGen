//
// _CoreGenEnv_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenEnv
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Environment Handlers
 *
 */

#ifndef _COREGENENV_H_
#define _COREGENENV_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class CoreGenEnv
{
private:
  CoreGenErrno *Errno;      ///< CoreGenEnv: Errno
  std::string ArchRoot;     ///< CoreGenEnv: Archive root directory

public:
  /// Default constructor
  CoreGenEnv(CoreGenErrno *E);

  /// Overloaded constructor
  CoreGenEnv(std::string ArchRoot, CoreGenErrno *E);

  /// Default destructor
  ~CoreGenEnv();

  /// Retrieve the Archive root directory
  std::string GetArchRoot() { return ArchRoot; }

  /// Retrieve the Plugin root directory
  std::string GetPluginRoot() { return ArchRoot + "/plugins"; }
};

#endif
// EOF
