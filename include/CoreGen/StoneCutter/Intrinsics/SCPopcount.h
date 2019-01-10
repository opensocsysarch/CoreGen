//
// _SCPopcount_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCPopcount
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Hamming Code (Popcount)
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCPOPCOUNT_H_
#define _STONECUTTER_SCINTRIN_SCPOPCOUNT_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCPopcount : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCPopcount();

  /// Overloaded Constructor
  SCPopcount( unsigned NI, std::string K );

  /// Default destructor
  ~SCPopcount();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
