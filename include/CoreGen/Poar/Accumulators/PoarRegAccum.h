//
// _PoarRegAccum_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarRegAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area accumulation register accumulation class
 *
 */

#ifndef _POARREGACCUM_H_
#define _POARREGACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarRegAccum : public PoarAccum {
private:
  /// PoarRegAccum: add the register to the data
  void AddLocalRegister(CoreGenReg *Reg);

  /// PoarRegAccum: accumulate by core
  bool SearchCores(std::vector<CoreGenNode *> &Cores);

  /// PoarRegAccum: accumulate by regclass
  bool SearchRegClasses();

public:
  /// PoarRegAccum: default constructor
  PoarRegAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"CoreGenReg") { }

  /// PoarRegAccum: default destructor
  ~PoarRegAccum() { }

  /// PoarRegAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
