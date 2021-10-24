//
// _SCClz_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCClz
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Count Leading Zeros
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCCLZ_H_
#define _STONECUTTER_SCINTRIN_SCCLZ_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCClz : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCClz();

  /// Overloaded Constructor
  SCClz( unsigned NI, std::string K );

  /// Default destructor
  ~SCClz();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
