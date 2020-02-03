//
// _CoreGenISA_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenDataPath
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Pipeline DataPath Handler
 *
 */

#ifndef _COREGENDATAPATH_H_
#define _COREGENDATAPATH_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

class CoreGenDataPath : public CoreGenNode{
private:
public:
  /// Default Constructor
  CoreGenDataPath(CoreGenErrno *Errno);

  /// Overloaded Constructor
  CoreGenDataPath( std::string Name, CoreGenErrno *Errno );

  /// Default Destructor
  ~CoreGenDataPath();
};

#endif

// EOF
