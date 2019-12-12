//
// _SCFence_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCFence
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Memory fence
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCFENCE_H_
#define _STONECUTTER_SCINTRIN_SCFENCE_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

//
// FENCE = Memory fence
//
// Format: FENCE()
//

class SCFence : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCFence();

  /// Overloaded Constructor
  SCFence( unsigned NI, std::string K );

  /// Default destructor
  ~SCFence();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
