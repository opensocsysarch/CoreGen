//
// _SCFieldIO_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCFieldIO
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Determines whether the instruction format are used correctly during I/O
 *
 */

#ifndef _STONECUTTER_SCPASS_SCFIELDIO_H_
#define _STONECUTTER_SCPASS_SCFIELDIO_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCFieldIO : public SCPass {
private:

  std::vector<std::string> Fields; ///< list of global immediate and encoding fields

  /// Checks the target instruction arguments for rogue writes to read-only fields
  bool CheckInstArgs( Function &F, Instruction &I );

  /// Retrieves all the immediate and encoding fields that we need to check
  bool RetrieveFields();

  /// Examines all instruction arguments that are instruction fields
  bool ExamineFieldIO();

public:
  /// Default cosntructor
  SCFieldIO(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCFieldIO();

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
