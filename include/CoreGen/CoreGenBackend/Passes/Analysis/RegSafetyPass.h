//
// _RegSafetyPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class RegSafetyPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Register Safety Pass
 *
 */

#ifndef _REGSAFETYPASS_H_
#define _REGSAFETYPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenReg.h"

class RegSafetyPass : public CoreGenPass{
private:

  /// Find the missing register indices
  bool FindMissingRegIndices(CoreGenReg *Reg);

  /// Find the registers whose indices exceed their target field width
  bool FindRegWidthRestrictions(CoreGenReg *Reg);

  /// Find overlapping SubReg names
  bool FindOverlappingSubRegNames(CoreGenReg *Reg);

  /// Find overlapping SubReg bit fields
  bool FindOverlappingSubRegFields(CoreGenReg *Reg);

  /// Test the SubReg values for various conditions
  bool TestSubRegs(CoreGenReg *Reg);

  /// Tests the target register classes for existence of multiple PCs
  bool TestPCRegs(CoreGenISA *ISA, std::vector<CoreGenRegClass *> RCs);

  /// Test the target ISA for existence of a single PC
  bool FindMultiplePC(CoreGenDAG *D, CoreGenISA *ISA);

  /// Retrieve the register class fields from the target inst formats
  std::vector<CoreGenRegClass *> GetRegClassFields( std::vector<CoreGenInstFormat *> IFs );

public:
  /// Default constructor
  RegSafetyPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~RegSafetyPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
