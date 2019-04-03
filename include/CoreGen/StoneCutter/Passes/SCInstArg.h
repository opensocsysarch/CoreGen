//
// _SCInstArg_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCInstArg
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Determines whether the instruction args match the register classes
 *
 */

#ifndef _STONECUTTER_SCPASS_SCINSTARG_H_
#define _STONECUTTER_SCPASS_SCINSTARG_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCInstArg : public SCPass {
private:
  std::map<std::string,std::string> RegClassMap;  ///< Map of registers to register classes
  std::vector<std::string> RegFileVect;           ///< Vector register files

  /// Retrieves a map of the registers to register classes from the global variable list
  bool GetRegClassMap();

  /// Walks the function list and ensures the argument lists are valid
  bool CheckInstArgs();

public:
  /// Default cosntructor
  SCInstArg(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCInstArg();

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
