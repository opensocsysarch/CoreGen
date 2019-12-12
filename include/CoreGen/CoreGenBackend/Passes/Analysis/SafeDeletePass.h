//
// _SafeDeletePass_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SafeDeletePass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Node Deletion Safety Test Pass
 *
 */

#ifndef _SAFEDELETEPASS_H_
#define _SAFEDELETEPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class SafeDeletePass : public CoreGenPass{
private:
public:
  /// Default constructor
  SafeDeletePass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~SafeDeletePass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
