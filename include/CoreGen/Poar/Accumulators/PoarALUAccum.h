//
// _PoarALUAccum_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarALUAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area accumulation ALU accumulation class
 *
 */

#ifndef _POARALUACCUM_H_
#define _POARALUACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarALUAccum : public PoarAccum {
private:

public:
  /// PoarALUAccum: default constructor
  PoarALUAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"ALU") { }

  /// PoarALUAccum: default destructor
  ~PoarALUAccum() { }

  /// PoarALUAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
