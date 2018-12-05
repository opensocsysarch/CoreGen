//
// _ASPSolverPass_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class ASPSolverPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Node Deletion Safety Test Pass
 *
 */

#ifndef _ASPSOLVERPASS_H_
#define _ASPSOLVERPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class ASPSolverPass : public CoreGenPass{
private:
  std::vector<std::string> Files;
public:
  /// Default constructor
  ASPSolverPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~ASPSolverPass();

  ///Set the files for each ASP Test
  void SetFiles(std::vector<std::string>);

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
