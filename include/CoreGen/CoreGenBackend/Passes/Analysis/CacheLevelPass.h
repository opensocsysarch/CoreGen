//
// _CacheLevelPass_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CacheLevelPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen ICache Checker Pass
 *
 */

#ifndef _CACHELEVELPASS_H_
#define _CACHELEVELPASS_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenCache.h"

class CacheLevelPass : public CoreGenPass{
private:
public:
  /// Default Constructor
  CacheLevelPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~CacheLevelPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
