//
// _PoarROMAccum_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarROMAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area ROM accumulation class
 *
 */

#ifndef _POARROMACCUM_H_
#define _POARROMACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarROMAccum : public PoarAccum {
private:

public:
  /// PoarROMAccum: default constructor
  PoarROMAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"CoreGenROM") { }

  /// PoarROMAccum: default destructor
  ~PoarROMAccum() { }

  /// PoarROMAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
