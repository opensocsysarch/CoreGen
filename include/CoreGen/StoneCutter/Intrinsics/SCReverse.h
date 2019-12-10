//
// _SCReverse_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCReverse
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Bit Reversal
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCREVERSE_H_
#define _STONECUTTER_SCINTRIN_SCREVERSE_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCReverse : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCReverse();

  /// Overloaded Constructor
  SCReverse( unsigned NI, std::string K );

  /// Default destructor
  ~SCReverse();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
