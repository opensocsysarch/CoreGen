//
// _SCSext_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCSext
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Sign Extension SEXT( VALUE, BASE )
 *        where VALUE is the target to sign extend and BASE is the sign bit
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCSEXT_H_
#define _STONECUTTER_SCINTRIN_SCSEXT_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCSext : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCSext();

  /// Overloaded Constructor
  SCSext( unsigned NI, std::string K );

  /// Default destructor
  ~SCSext();

  /// Execute the codegen
  virtual Value *codegen();

  /// Retrieve the signal map
  bool GetSigMap(CoreGenSigMap *Sigs,
                 Instruction &I,
                 std::string Inst);
};

#endif

// EOF
