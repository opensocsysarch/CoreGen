//
// _SCNop_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCNop
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: NOP; No Operation
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCNOP_H_
#define _STONECUTTER_SCINTRIN_SCNOP_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCNop : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCNop();

  /// Overloaded Constructor
  SCNop( unsigned NI, std::string K );

  /// Default destructor
  ~SCNop();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
