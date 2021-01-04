//
// _PoarDPathAccum_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarDPathAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area data path accumulation class
 *
 */

#ifndef _POARDPATHACCUM_H_
#define _POARDPATHACCUM_H_

#include "CoreGen/Poar/PoarAccum.h"

class PoarDPathAccum : public PoarAccum {
private:
  /// PoarDPathAccum: Searches the memory data path
  bool SearchMemoryDataPath();

  /// PoarDPathAccum: Searches the instruction data path
  bool SearchInstructionDataPath();

  /// PoarDPathAccum: Searches the register data path
  bool SearchRegisterDataPath();

  /// PoarDPathAccum: Searches the cache data path
  bool SearchCacheDataPath();

  /// PoarDPathAccum: Searches the Core to Cache data path
  bool SearchCoreToCache();

  /// PoarDPathAccum: Searches within the cache hierarchy
  bool SearchCacheLevels();

public:
  /// PoarDPathAccum: default constructor
  PoarDPathAccum(CoreGenNode *T,CoreGenSigMap *S)
    : PoarAccum(T,S,"CoreGenDPath") { }

  /// PoarDPathAccum: default destructor
  ~PoarDPathAccum() { }

  /// PoarDPathAccum: execute the accumulator
  virtual bool Accum() override;
};

#endif

// EOF
