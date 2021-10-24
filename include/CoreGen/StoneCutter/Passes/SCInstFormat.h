//
// _SCInstFormat_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCInstFormat
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Determines whether the instruction format args match the register classes
 *
 */

#ifndef _STONECUTTER_SCPASS_SCINSTFORMAT_H_
#define _STONECUTTER_SCPASS_SCINSTFORMAT_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCInstFormat : public SCPass {
private:
  std::vector<std::string> RegFileVect;           ///< Vector register files

  /// Retrieves a vector of register file names to check against instruction format reg fields
  bool GetRegFileVect();

  /// Checks the register file vector against the instruction format fields
  bool CheckInstFormat();

public:
  /// Default cosntructor
  SCInstFormat(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCInstFormat();

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
