//
// _EncodingCollisionPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _ENCODINGCOLLISIONPASS_H_
#define _ENCODINGCOLLISIONPASS_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"

/**
 * \class ENCLongInt
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen long integer class for EncodingCollisionPass
 *
 */

class ENCLongInt{
private:
  unsigned Width;   ///< ENCLongInt: Max instruction width
  unsigned NVals;   ///< ENCLongInt: Number of u64 values
  uint64_t *Vals;   ///< ENCLongInt: Array of u64 values

public:
  /// Default constructor
  ENCLongInt( unsigned W, unsigned N ) {
    // setup the initial values
    Width = W;
    NVals = N;
    Vals = new uint64_t[NVals];

    // zero all the values
    for( unsigned i=0; i<NVals; i++ ){
      Vals[i] = 0x00ull;
    }

    // set our mask for trailing bits where the
    // encoding format is not a perfect divisor of 64
    unsigned MaskBits = (NVals*64) - Width;
    for( unsigned i=0; i<MaskBits; i++ ){
      Vals[NVals-1] |= (uint64_t)(1<<(64-(i+1)));
    }
  }

  /// Set the encoding mask
  bool SetMask( unsigned Start, unsigned End, uint64_t Val ){
    unsigned Pos = 0;
    unsigned LBit = Start;
    if( Start > 63 ){
      Pos = Start%64;
    }
    if( Pos > 0 ){
      LBit = Start-(Pos*64);
    }
    for( unsigned i=0; i<(End-Start); i++ ){
      Vals[Pos] |= (((Val & (1<<i))>>i)<<(LBit+i));
    }
    return true;
  }

  /// Default destructor
  ~ENCLongInt() { delete [] Vals; }

  /// Get the target bit
  unsigned GetBit( unsigned bit ){
    unsigned Pos = 0;
    unsigned LBit = bit;
    if( bit > 63 ){
      Pos = bit%64;
    }
    if( Pos > 0 ){
      LBit = bit-(Pos*64);
    }
    return (unsigned)((Vals[Pos] & (1<<LBit))>>LBit);
  }

  /// Get the instruction width
  unsigned GetWidth() { return Width; }

  /// ENCLongInt overlaoded operator for LESS THAN
  friend bool operator<(ENCLongInt s1, ENCLongInt s2){
    for( unsigned i=0; i<s1.GetWidth(); i++ ){
      if( s1.GetBit(i) < s2.GetBit(i) ){
        return true;
      }
    }
    return false;
  }

  /// ENCLongInt overloaded operator for EQUAL TO
  friend bool operator==(ENCLongInt s1, ENCLongInt s2){
    for( unsigned i=0; i<s1.GetWidth(); i++ ){
      if( s1.GetBit(i) != s2.GetBit(i) ){
        return false;
      }
    }
    return true;
  }

};


/**
 * \class EncodingCollisionPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Encoding Collision Checker Pass
 *
 */
class EncodingCollisionPass : public CoreGenPass{
private:
  /// Examines an ISA node for encoding collisions
  bool ExamineISANode(CoreGenDAG *D, unsigned IsaNode );

  /// Processes 64bit instructions
  bool Process64bitInsts(std::vector<CoreGenInst *> Insts);

  /// Processes longer instructions
  bool ProcessLongInsts(std::vector<CoreGenInst *> Insts);
public:
  /// Default Constructor
  EncodingCollisionPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~EncodingCollisionPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
