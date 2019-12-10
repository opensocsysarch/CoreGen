//
// _CoreGenSoC_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenSoC
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen SoC Handler
 *
 */

#ifndef _COREGENSOC_H_
#define _COREGENSOC_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenCore.h"

class CoreGenSoC : public CoreGenNode{
private:
  std::vector<CoreGenCore *> Cores;   ///< CoreGenSoC: list of attached cores

public:
  /// Default constructor
  CoreGenSoC(std::string Name, CoreGenErrno *E);

  /// Default destructor
  ~CoreGenSoC();

  /// Insert a core into the SoC
  bool InsertCore( CoreGenCore *C );

  /// Deletes the target core
  bool DeleteCore( CoreGenCore *C );

  /// Retrieve the target core from the SoC
  CoreGenCore *GetCore( unsigned C );

  /// Retrieve the number of cores in the SoC
  unsigned GetNumCores() { return Cores.size(); }
};

#endif

// EOF
