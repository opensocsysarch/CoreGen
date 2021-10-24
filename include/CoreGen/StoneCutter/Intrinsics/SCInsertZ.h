//
// _SCInsertZ_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCInsertZ
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Insert and zero extend
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCINSERTZ_H_
#define _STONECUTTER_SCINTRIN_SCINSERTZ_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCInsertZ : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCInsertZ();

  /// Overloaded Constructor
  SCInsertZ( unsigned NI, std::string K );

  /// Default destructor
  ~SCInsertZ();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
