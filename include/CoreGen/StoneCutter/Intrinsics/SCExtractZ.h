//
// _SCExtractZ_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCExtractZ
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Extract and zero extend
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCEXTRACTZ_H_
#define _STONECUTTER_SCINTRIN_SCEXTRACTZ_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCExtractZ : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCExtractZ();

  /// Overloaded Constructor
  SCExtractZ( unsigned NI, std::string K );

  /// Default destructor
  ~SCExtractZ();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
