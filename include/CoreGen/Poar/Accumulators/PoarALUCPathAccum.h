//
// _PoarALUCPathAccum_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarALUCPathAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area ALU control path accumulation class
 *
 */

#ifndef _POARALUCPATHACCUM_H_
#define _POARALUCPATHACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarALUCPathAccum : public PoarAccum {
private:

public:
  /// PoarALUCPathAccum: default constructor
  PoarALUCPathAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"ALUCPath") { }

  /// PoarALUCPathAccum: default destructor
  ~PoarALUCPathAccum() { }

  /// PoarALUCPathAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
