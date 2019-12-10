//
// _CacheLevelOpt_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CacheLevelOpt
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Cache Level Layout Optimization
 *
 */


#ifndef _CACHELEVELOPT_H_
#define _CACHELEVELOPT_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenCache.h"

class CacheLevelOpt : public CoreGenPass{
private:

public:
  /// Default constructor
  CacheLevelOpt(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~CacheLevelOpt();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
