//
// _SCLoadElem_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCLoadElem
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Load a Value From Memory Using Specific Element Size
 *        TARGET = LOAD(ADDR,SIZE)
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCLOADELEM_H_
#define _STONECUTTER_SCINTRIN_SCLOADELEM_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCLoadElem : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCLoadElem();

  /// Overloaded Constructor
  SCLoadElem( unsigned NI, std::string K );

  /// Default destructor
  ~SCLoadElem();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
