//
// _VLIWSafetyPass_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class VLIWSafetyPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen VLIW Stage Safety Pass
 *
 */

#ifndef _VLIWSAFETYPASS_H_
#define _VLIWSAFETYPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenVLIWStage.h"

class VLIWSafetyPass : public CoreGenPass{
private:
  /// Check the VLIW Instruction Format
  bool CheckInstFormat(CoreGenDAG *D);

public:
  /// Default constructor
  VLIWSafetyPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~VLIWSafetyPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
