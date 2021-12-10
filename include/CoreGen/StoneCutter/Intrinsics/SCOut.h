//
// _SCOut_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCOut
 *
 * \ingroup StoneCutter
 *
 * \brief VLIW Out
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCOUT_H_
#define _STONECUTTER_SCINTRIN_SCOUT_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCOut : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCOut();

  /// Overloaded Constructor
  SCOut( unsigned NI, std::string K );

  /// Default destructor
  ~SCOut();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
