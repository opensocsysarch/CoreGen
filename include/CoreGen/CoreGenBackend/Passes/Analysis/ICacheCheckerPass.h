//
// _ICacheCheckerPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class ICacheCheckerPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen ICache Checker Pass
 *
 */

#ifndef _ICACHECHECKERPASS_H_
#define _ICACHECHECKERPASS_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class ICacheCheckerPass : public CoreGenPass{
private:
public:
  /// Default Constructor
  ICacheCheckerPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~ICacheCheckerPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
