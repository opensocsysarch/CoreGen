//
// _PoarCPathAccum_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarCPathAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area data path accumulation class
 *
 */

#ifndef _POARCPATHACCUM_H_
#define _POARCPATHACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarCPathAccum : public PoarAccum {
private:
  /// PoarCPathAccum: Searches the memory data path
  bool SearchMemoryCtrlPath();

  /// PoarCPathAccum: Searches the instruction data path
  bool SearchInstructionCtrlPath();

  /// PoarCPathAccum: Searches the register data path
  bool SearchRegisterCtrlPath();

  /// PoarCPathAccum: Searches the cache data path
  bool SearchCacheCtrlPath();

  /// PoarCPathAccum: Searches the Core to Cache data path
  bool SearchCoreToCache();

  /// PoarCPathAccum: Searches within the cache hierarchy
  bool SearchCacheLevels();

public:
  /// PoarCPathAccum: default constructor
  PoarCPathAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"CoreGenCPath") { }

  /// PoarCPathAccum: default destructor
  ~PoarCPathAccum() { }

  /// PoarCPathAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
