//
// _SCRotateR_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCRotateR
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Bitwise Rotate Right
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCROTATER_H_
#define _STONECUTTER_SCINTRIN_SCROTATER_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCRotateR : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCRotateR();

  /// Overloaded Constructor
  SCRotateR( unsigned NI, std::string K );

  /// Default destructor
  ~SCRotateR();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
