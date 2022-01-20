//
// _PoarALUDPathAccum_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarALUDPathAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area ALU data path accumulation class
 *
 */

#ifndef _POARALUDPATHACCUM_H_
#define _POARALUDPATHACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarALUDPathAccum : public PoarAccum {
private:

public:
  /// PoarALUDPathAccum: default constructor
  PoarALUDPathAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"ALUDPath") { }

  /// PoarALUDPathAccum: default destructor
  ~PoarALUDPathAccum() { }

  /// PoarALUDPathAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
