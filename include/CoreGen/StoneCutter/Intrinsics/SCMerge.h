//
// _SCMerge_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCMerge
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Merge under mask: r = a ^ ((a ^ b) & mask)
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCMERGE_H_
#define _STONECUTTER_SCINTRIN_SCMERGE_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCMerge : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCMerge();

  /// Overloaded Constructor
  SCMerge( unsigned NI, std::string K );

  /// Default destructor
  ~SCMerge();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
