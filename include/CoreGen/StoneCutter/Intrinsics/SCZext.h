//
// _SCZext_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCZext
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Zero Extension: ZEXT( VALUE, BASE )
 *        where VALUE is the target to zero extend and BASE is the most significant bit
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCZEXT_H_
#define _STONECUTTER_SCINTRIN_SCZEXT_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCZext : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCZext();

  /// Overloaded Constructor
  SCZext( unsigned NI, std::string K );

  /// Default destructor
  ~SCZext();

  /// Execute the codegen
  virtual Value *codegen();

  /// Retrieve the signal map
  bool GetSigMap(CoreGenSigMap *Sigs,
                 Instruction &I,
                 std::string Inst);
};

#endif

// EOF
