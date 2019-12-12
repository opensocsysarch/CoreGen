//
// _SCLss_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCLss
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Least significant bit select
 *                               Selects the least significant bits up to the
 *                               last positive '1' bit
 *
 *        Ex: LSS(0b000101010) yields 0b101010
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCLSS_H_
#define _STONECUTTER_SCINTRIN_SCLSS_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCLss : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCLss();

  /// Overloaded Constructor
  SCLss( unsigned NI, std::string K );

  /// Default destructor
  ~SCLss();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
