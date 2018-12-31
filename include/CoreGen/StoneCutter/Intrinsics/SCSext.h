//
// _SCSext_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCINTRIN_SCSEXT_H_
#define _STONECUTTER_SCINTRIN_SCSEXT_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCSext : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCSext();

  /// Overloaded Constructor
  SCSext( unsigned NI, std::string K );

  /// Default destructor
  ~SCSext();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
