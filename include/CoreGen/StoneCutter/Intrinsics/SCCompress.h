//
// _SCCompress_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCCompress
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Bit Compression
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCCOMPRESS_H_
#define _STONECUTTER_SCINTRIN_SCCOMPRESS_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

//
// COMPRESS = Bit Compression
//
// Format: COMPRESS(x)
//
// Performs a bit compression of the input value
// For 'N' 1 bits in the input value, returns a
// value with 'N' bits set in the least significant bits
//
// EX: COMPRESS(0b1010101) = 0b1111
//

class SCCompress : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCCompress();

  /// Overloaded Constructor
  SCCompress( unsigned NI, std::string K );

  /// Default destructor
  ~SCCompress();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
