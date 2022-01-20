//
// _SCBsel_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCBsel
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Bit Select: RT = BSEL(RA,START,END)
 *                    Selects the bit field starting at START and ending at END from RA
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCBSEL_H_
#define _STONECUTTER_SCINTRIN_SCBSEL_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCBsel : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCBsel();

  /// Overloaded Constructor
  SCBsel( unsigned NI, std::string K );

  /// Default destructor
  ~SCBsel();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
