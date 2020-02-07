//
// _RegClassSafetyPass_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class RegClassSafetyPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Register Class Safety Class
 *
 */

#ifndef _REGCLASSSAFETYPASS_H_
#define _REGCLASSSAFETYPASS_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"
#include "CoreGen/CoreGenBackend/CoreGenReg.h"

class RegClassSafetyPass : public CoreGenPass{
private:
  /// Determines if the register class is empty
  bool EmptyRegClass( CoreGenNode *N );

  /// Determines if there are collisions in the register names
  bool RegNameCollision( CoreGenNode *N );

  /// Determines if there are collisions in the register pseudonames
  bool PseudoNameCollision( CoreGenNode *N );

  /// Determines if the register class has inefficient register sizes
  bool RegSizeTest( CoreGenNode *N );

  /// Determines if the register class has sufficient read and write ports
  bool ReadWritePortTest( CoreGenNode *N );

public:
  /// Default Constructor
  RegClassSafetyPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~RegClassSafetyPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
