//
// _RegIdxPass_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class RegIdxPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Register Index Collision Pass
 *
 */

#ifndef _REGIDXPASS_H_
#define _REGIDXPASS_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cstdlib>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenReg.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"

class RegIdxPass : public CoreGenPass{
private:
public:
  /// Default constructor
  RegIdxPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~RegIdxPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
