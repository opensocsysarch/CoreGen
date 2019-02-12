//
// _SCPipeBuilder_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCPipeBuilder
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Constructs the arithmetic portions of the Chisel pipeline
 *
 */

#ifndef _STONECUTTER_SCPASS_SCPIPEBUILDER_H_
#define _STONECUTTER_SCPASS_SCPIPEBUILDER_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCPipeBuilder : public SCPass {
private:

  std::vector<unsigned> ArithOps;   ///< Pipline arithmetic operations
  std::vector<unsigned> ShiftOps;   ///< Pipline shift operations

  /// Construct the arithmetic pipeline
  bool BuildArithPipe();

public:
  /// Default cosntructor
  SCPipeBuilder(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCPipeBuilder();

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
