//
// _CommSafetyPass_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CommSafetyPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Pseudo Instruction Safety Pass
 *
 */

#ifndef _COMMSAFETYPASS_H_
#define _COMMSAFETYPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenComm.h"

class CommSafetyPass : public CoreGenPass{
private:
  bool CheckCommCompat(CoreGenDAG *D);
  bool CheckCommCompatP2P(CoreGenDAG *D, unsigned Root);
  bool CheckCommCompatBus(CoreGenDAG *D, unsigned Root);
  bool CheckCommCompatNOC(CoreGenDAG *D, unsigned Root);
  bool CheckCommCompatUnk(CoreGenDAG *D, unsigned Root);
public:
  /// Default constructor
  CommSafetyPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~CommSafetyPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
