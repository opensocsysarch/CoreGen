//
// _SCNot_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCNot
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Bitwise Complement
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCNOT_H_
#define _STONECUTTER_SCINTRIN_SCNOT_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCNot : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCNot();

  /// Overloaded Constructor
  SCNot( unsigned NI, std::string K );

  /// Default destructor
  ~SCNot();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
