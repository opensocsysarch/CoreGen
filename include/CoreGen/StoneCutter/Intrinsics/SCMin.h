//
// _SCMin_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCMin
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Two Input Minimum
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCMIN_H_
#define _STONECUTTER_SCINTRIN_SCMIN_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCMin : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCMin();

  /// Overloaded Constructor
  SCMin( unsigned NI, std::string K );

  /// Default destructor
  ~SCMin();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
