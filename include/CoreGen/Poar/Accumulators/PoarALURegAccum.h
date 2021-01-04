//
// _PoarALURegAccum_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarALURegAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area accumulation ALU register accumulation class
 *
 */

#ifndef _POARALUREGACCUM_H_
#define _POARALUREGACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarALURegAccum : public PoarAccum {
private:

public:
  /// PoarALURegAccum: default constructor
  PoarALURegAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"ALUReg") { }

  /// PoarALURegAccum: default destructor
  ~PoarALURegAccum() { }

  /// PoarALURegAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
