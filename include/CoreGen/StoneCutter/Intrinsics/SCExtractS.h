//
// _SCExtractS_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCExtractS
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Extract and sign extend
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCEXTRACTS_H_
#define _STONECUTTER_SCINTRIN_SCEXTRACTS_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCExtractS : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCExtractS();

  /// Overloaded Constructor
  SCExtractS( unsigned NI, std::string K );

  /// Default destructor
  ~SCExtractS();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
