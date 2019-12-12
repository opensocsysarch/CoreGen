//
// _CoreGenCache_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenCache
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Cache Node Handler
 *
 */

#ifndef _COREGENCACHE_H_
#define _COREGENCACHE_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class CoreGenCache : public CoreGenNode{
private:
  unsigned Sets;        ///< CoreGenCache: Number of sets
  unsigned Ways;        ///< CoreGenCache: Number of ways
  bool SubLevel;        ///< CoreGenCache: Is this a sublevel?
  bool ParentLevel;     ///< CoreGenCache: Is this a parent level?
  CoreGenCache *Child;  ///< CoreGenCache: Child cache level
  std::vector<CoreGenCache *> Parent; ///< CoreGenCache: Vector of parent cache level

public:

  /// Default constructor
  CoreGenCache(std::string Name, CoreGenErrno *E);

  /// Overloaded constructor
  CoreGenCache(std::string Name, unsigned CSets,
               unsigned CWays, CoreGenErrno *E);

  /// Retrieve the number of sets
  unsigned GetSets() { return Sets; }

  /// Retrieve the number of ways
  unsigned GetWays() { return Ways; }

  /// Set the number of sets
  bool SetSets( unsigned S ) { Sets = S; return true; }

  /// Set the number of ways
  bool SetWays( unsigned W ) { Ways = W; return true; }

  /// Retrieve whether this is a sublevel cache
  bool IsSubLevel() { return SubLevel; }

  /// Retrieve whether there is a child level
  bool IsParentLevel() { return ParentLevel; }

  /// Sets a sublevel cache level
  bool SetChildCache( CoreGenCache *C );

  /// Sets a parent cache level
  bool SetParentCache( CoreGenCache *C );

  /// Sets the parent cache to null
  bool SetNullChildCache();

  /// Sets the sublevel cache to null
  bool SetNullParentCache();

  /// Retrieve a pointer to a cache sublevel
  CoreGenCache *GetSubCache();

  /// Retrieve a pointer to a cache parent
  CoreGenCache *GetParentCache(unsigned Idx);

  /// Delete a parent cache
  void DeleteParentCache(CoreGenCache *C);

  /// Retrieve the number of parent caches
  unsigned GetNumParentCache() { return Parent.size(); }

  /// Determines if the target cache level is a parent to this cache
  bool HasParentCache( CoreGenCache *P );

  // Default destructor
  ~CoreGenCache();
};

#endif

// EOF
