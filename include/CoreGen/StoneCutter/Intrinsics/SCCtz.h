//
// _SCCtz_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCCtz
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Count Trailing Zeros
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCCTZ_H_
#define _STONECUTTER_SCINTRIN_SCCTZ_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCCtz : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCCtz();

  /// Overloaded Constructor
  SCCtz( unsigned NI, std::string K );

  /// Default destructor
  ~SCCtz();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
