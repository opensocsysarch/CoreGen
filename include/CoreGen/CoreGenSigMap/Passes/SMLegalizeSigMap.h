//
// _SMLegalizeSigMap_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SMLegalizeSigMap
 *
 * \ingroup CoreGenSigMap
 *
 * \brief CoreGenSigMap Pass: Ensures the signal map is correct and stable
 *
 */

#ifndef _COREGENSIGMAP_SMPASS_SMLEGALIZESIGMAP_H_
#define _COREGENSIGMAP_SMPASS_SMLEGALIZESIGMAP_H_

#include "CoreGen/CoreGenSigMap/SMPass.h"

class SMLegalizeSigMap : public SMPass {
private:
  std::vector<std::string> SigNames;        ///< vector of signal names

  /// Checks the last uOp for forward branches
  bool CheckForwardBr();

  /// Checks the first uOp for backend branches
  bool CheckBackwardBr();

  /// Checks all branches to ensure that remain within the uOp block
  bool CheckLegalBr();

  /// Builds the internal vector of instructions
  bool GetInstVect();

  /// returns a vector of signals for the target instructon
  std::vector<SCSig *> GetSigVect(std::string Inst);

public:
  /// Default constructor
  SMLegalizeSigMap(std::string N,
                   std::vector<SCSig *> SigVect,
                   std::vector<SCTmp *> TempVect,
                   std::vector<SigType> TopVect);

  /// Default destructor
  ~SMLegalizeSigMap();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
