//
// _MultSoCPass_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class MultSoCPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Multiple SoC Pass
 *
 */

#ifndef _MULTSOCPASS_H_
#define _MULTSOCPASS_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class MultSoCPass : public CoreGenPass{
private:
public:
  /// Default Constructor
  MultSoCPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~MultSoCPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
