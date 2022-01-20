//
// _SCNeg_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCNeg
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Negation
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCNEG_H_
#define _STONECUTTER_SCINTRIN_SCNEG_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCNeg : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCNeg();

  /// Overloaded Constructor
  SCNeg( unsigned NI, std::string K );

  /// Default destructor
  ~SCNeg();

  /// Execute the codegen
  virtual Value *codegen();

  /// Retrieve the signal map
  bool GetSigMap(CoreGenSigMap *Sigs,
                 Instruction &I,
                 std::string Inst);
};

#endif

// EOF
