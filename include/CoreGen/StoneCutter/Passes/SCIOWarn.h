//
// _SCIOWarn_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCIOWarn
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Warns on register file accesses outside of
 *                                  the standard I/O mechanisms
 *
 */

#ifndef _STONECUTTER_SCPASS_SCIOWARN_H_
#define _STONECUTTER_SCPASS_SCIOWARN_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCIOWarn : public SCPass {
private:

  ///< Checks the target load/store instruction against the instruction's protoype list
  void CheckPrototypeIO( Function &F, Instruction &I );

  ///< Walks all instructions and examines the I/O blocks for safety
  void CheckIOLayout();

public:
  /// Default cosntructor
  SCIOWarn(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCIOWarn();

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
