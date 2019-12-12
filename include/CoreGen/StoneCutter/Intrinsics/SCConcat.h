//
// _SCConcat_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCConcat
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Concatenates two input variables
 *                               A = CONCAT(B,C)
 *                               Where the size of A = size(B) + size(C)
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCCONCAT_H_
#define _STONECUTTER_SCINTRIN_SCCONCAT_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCConcat : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCConcat();

  /// Overloaded Constructor
  SCConcat( unsigned NI, std::string K );

  /// Default destructor
  ~SCConcat();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
