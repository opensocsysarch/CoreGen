//
// _SCRotateL_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCINTRIN_SCROTATEL_H_
#define _STONECUTTER_SCINTRIN_SCROTATEL_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCRotateL : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCRotateL();

  /// Overloaded Constructor
  SCRotateL( unsigned NI, std::string K );

  /// Default destructor
  ~SCRotateL();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
