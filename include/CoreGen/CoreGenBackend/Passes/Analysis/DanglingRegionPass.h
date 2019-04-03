//
// _DanglingRegionPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class DanglingRegionPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Dangling Region Pass
 *
 */

#ifndef _DANGLINGREGIONPASS_H_
#define _DANGLINGREGIONPASS_H_

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class DanglingRegionPass : public CoreGenPass{
private:

  /// Performs a Depth-First-Search
  void DFS( CoreGenDAG *DAG, unsigned DimSize, unsigned v, bool visited[] );

public:
  /// Default Constructor
  DanglingRegionPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~DanglingRegionPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
