//
// _PoarCacheAccum_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarCacheAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area accumulation cache accumulation class
 *
 */

#ifndef _POARCACHEACCUM_H_
#define _POARCACHEACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarCacheAccum : public PoarAccum {
private:

public:
  /// PoarCacheAccum: default constructor
  PoarCacheAccum(CoreGenNode *T, CoreGenSigMap *S)
    : PoarAccum(T,S,"CoreGenCache") { }

  /// PoarCacheAccum: default destructor
  ~PoarCacheAccum() { }

  /// PoarCacheAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
