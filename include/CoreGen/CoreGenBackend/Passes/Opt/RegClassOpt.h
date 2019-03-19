//
// _RegClassOpt_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class RegClassOpt
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Register Class Layout Optimization
 *
 */


#ifndef _REGCLASSOPT_H_
#define _REGCLASSOPT_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"

class RegClassOpt : public CoreGenPass{
private:
public:
  /// Default constructor
  RegClassOpt(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~RegClassOpt();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
