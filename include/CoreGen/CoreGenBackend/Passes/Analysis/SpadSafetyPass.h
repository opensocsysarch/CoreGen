//
// _SpadSafetyPass_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SpadSafetyPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Scratchpad Safety Pass
 *
 */

#ifndef _SPADSAFETYPASS_H_
#define _SPADSAFETYPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenSpad.h"

class SpadSafetyPass : public CoreGenPass{
private:

  std::vector<CoreGenSpad *> Spads; ///< Scratchpad vector

  /// Checks the address collisions across scratchpads
  bool TestAddressCollisions();

  /// Checks the scratchpad port continuity
  bool TestPortConfigs();

public:
  /// Default constructor
  SpadSafetyPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~SpadSafetyPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
