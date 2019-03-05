//
// _PInstSafetyPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PInstSafetyPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Pseudo Instruction Safety Pass
 *
 */

#ifndef _PINSTSAFETYPASS_H_
#define _PINSTSAFETYPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenPseudoInst.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"

class PInstSafetyPass : public CoreGenPass{
private:
  bool CheckISACompat(CoreGenDAG *D);
public:
  /// Default constructor
  PInstSafetyPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~PInstSafetyPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
