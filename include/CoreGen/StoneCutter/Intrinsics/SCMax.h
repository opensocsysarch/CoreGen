//
// _SCMax_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCINTRIN_SCMAX_H_
#define _STONECUTTER_SCINTRIN_SCMAX_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCMax : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCMax();

  /// Overloaded Constructor
  SCMax( unsigned NI, std::string K );

  /// Default destructor
  ~SCMax();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF