//
// _SCVLIW_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCVLIW
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Warns on register file accesses outside of
 *                                  the standard I/O mechanisms
 *
 */

#ifndef _STONECUTTER_SCPASS_SCVLIW_H_
#define _STONECUTTER_SCPASS_SCVLIW_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCVLIW : public SCPass {
private:

  ///< Walks all instructions and examines the I/O blocks for safety
  void BundleMaker(Function &F, Instruction &I);

public:
  /// Default cosntructor
  SCVLIW(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCVLIW();

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
