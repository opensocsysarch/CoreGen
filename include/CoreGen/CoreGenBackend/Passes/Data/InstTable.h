//
// _InstTable_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class InstTable
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen InstTable Pass
 *
 */

#ifndef _INSTTABLE_H_
#define _INSTTABLE_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class InstTable : public CoreGenPass{
private:
public:
  /// Default constructor
  InstTable(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~InstTable();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
