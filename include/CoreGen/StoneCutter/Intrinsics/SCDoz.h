//
// _SCDoz_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCINTRIN_SCDOZ_H_
#define _STONECUTTER_SCINTRIN_SCDOZ_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

//
// DOZ = Difference Or Zero
//
// Format: DOZ(x,y)
//
// Performs "first grade subtraction" of {x-y}
// Returns {x-y} IFF x >= y; Otherwise returns 0
//

class SCDoz : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCDoz();

  /// Overloaded Constructor
  SCDoz( unsigned NI, std::string K );

  /// Default destructor
  ~SCDoz();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
