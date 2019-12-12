//
// _SCCompressM_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCCompressM
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Bit Compression Under Mask
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCCOMPRESSM_H_
#define _STONECUTTER_SCINTRIN_SCCOMPRESSM_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

//
// COMPRESSM = Bit Compress Under Mask
//
// Format: COMPRESSM(x,mask)
//
// Performs a bit compression of the input value
// For 'N' 1 bits in the input value whose equivalent value is enabled in the 
// mask, returns a value with 'N' bits set in the least significant bits
//
// EX: COMPRESS(0b1010101,0b111) = 0b0000011
//

class SCCompressM : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCCompressM();

  /// Overloaded Constructor
  SCCompressM( unsigned NI, std::string K );

  /// Default destructor
  ~SCCompressM();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
