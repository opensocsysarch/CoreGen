//
// _SCLoad_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCLoad
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Laod a Value From Memory
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCLOAD_H_
#define _STONECUTTER_SCINTRIN_SCLOAD_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCLoad : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCLoad();

  /// Overloaded Constructor
  SCLoad( unsigned NI, std::string K );

  /// Default destructor
  ~SCLoad();

  /// Execute the codegen
  virtual Value *codegen();

  /// Retrieve the signal map
  virtual bool GetSigMap(CoreGenSigMap *Sigs,
                         Instruction& I,
                         std::string Inst);
};

#endif

// EOF
