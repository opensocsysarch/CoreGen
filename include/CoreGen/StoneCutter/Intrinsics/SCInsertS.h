//
// _SCInsertS_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCInsertS
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Insert and sign extend
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCINSERTS_H_
#define _STONECUTTER_SCINTRIN_SCINSERTS_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCInsertS : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCInsertS();

  /// Overloaded Constructor
  SCInsertS( unsigned NI, std::string K );

  /// Default destructor
  ~SCInsertS();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
