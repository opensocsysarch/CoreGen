//
// _L1SharedPass_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class L1SharedPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen L1 Shared Cache Checker Pass
 *
 */

#ifndef _L1SHAREDPASS_H_
#define _L1SHAREDPASS_H_

#include <iostream>
#include <string>
#include <map>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenCache.h"

class L1SharedPass : public CoreGenPass{
private:
public:
  /// Default Constructor
  L1SharedPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~L1SharedPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
