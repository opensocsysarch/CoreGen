//
// _ASPSolverPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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
 * \brief CoreGen Integrated ASP Constaint Solver Pass
 *
 */

#ifndef _ASPSOLVERPASS_H_
#define _ASPSOLVERPASS_H_

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

// clingo/gringo headers
#ifdef CLINGO_WITH_PYTHON
#   include <pyclingo.h>
#endif
#ifdef CLINGO_WITH_LUA
#   include <luaclingo.h>
#endif

#include <clingo.h>

extern "C" CLINGO_VISIBILITY_DEFAULT int clingo_main_(int argc, char *argv[]);

class ASPSolverPass : public CoreGenPass{
private:
  std::vector<std::string> Files;   ///< List of files as input
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
