//
// _DanglingNodePass_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class DanglingNodePass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Dangling Node Pass
 *
 */

#ifndef _DANGLINGNODEPASS_H_
#define _DANGLINGNODEPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class DanglingNodePass : public CoreGenPass{
private:
public:
  /// Default Constructor
  DanglingNodePass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~DanglingNodePass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
