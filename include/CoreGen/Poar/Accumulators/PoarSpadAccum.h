//
// _PoarSpadAccum_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarSpadAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area accumulation scratchpad accumulation class
 *
 */

#ifndef _POARSPADACCUM_H_
#define _POARSPADACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarSpadAccum : public PoarAccum {
private:

public:
  /// PoarSpadAccum: default constructor
  PoarSpadAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"CoreGenSpad") { }

  /// PoarSpadAccum: default destructor
  ~PoarSpadAccum() { }

  /// PoarSpadAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
