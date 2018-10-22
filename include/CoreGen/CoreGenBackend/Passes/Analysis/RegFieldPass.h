//
// _RegFieldPass_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class RegFieldPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Register Field Encoding Pass
 *
 */

#ifndef _REGFIELDPASS_H_
#define _REGFIELDPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"

class RegFieldPass : public CoreGenPass{
private:
public:
  /// Default constructor
  RegFieldPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~RegFieldPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
