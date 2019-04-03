//
// _CoreSafetyPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreSafetyPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Core Safety Pass
 *
 */

#ifndef _CORESAFETYPASS_H_
#define _CORESAFETYPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenCore.h"

class CoreSafetyPass : public CoreGenPass{
private:
  /// Searches for unconnected Cores
  bool FindDanglingCores(CoreGenDAG *D, CoreGenCore *Core, unsigned Idx);

public:
  /// Default constructor
  CoreSafetyPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~CoreSafetyPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
