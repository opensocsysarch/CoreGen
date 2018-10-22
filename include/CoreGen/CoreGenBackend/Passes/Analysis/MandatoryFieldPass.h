//
// _MandatoryFieldPass_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class MandatoryFieldPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Mandatory Encoding Field Pass
 *
 */

#ifndef _MANDATORYFIELDPASS_H_
#define _MANDATORYFIELDPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"

class MandatoryFieldPass : public CoreGenPass{
private:
public:
  /// Default Constructor
  MandatoryFieldPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~MandatoryFieldPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
