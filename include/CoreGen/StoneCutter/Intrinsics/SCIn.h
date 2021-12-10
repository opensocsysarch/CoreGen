//
// _SCIn_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCIn
 *
 * \ingroup StoneCutter
 *
 * \brief VLIW In
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCIN_H_
#define _STONECUTTER_SCINTRIN_SCIN_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCIn : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCIn();

  /// Overloaded Constructor
  SCIn( unsigned NI, std::string K );

  /// Default destructor
  ~SCIn();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
