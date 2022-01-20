//
// _CoreGenCore_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenCore
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Core Handler
 *
 */

#ifndef _COREGENCORE_H_
#define _COREGENCORE_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"
#include "CoreGen/CoreGenBackend/CoreGenCache.h"

typedef enum{
  SMTUnk      = 0,			///< CGSched: Unknown SMT Scheduling
  SMTPCycle   = 1,                      ///< CGSched: SMT P-Cycle Scheduling
  SMTPressure = 2,                      ///< CGSched: SMT Pressure-Based Scheduling
  SMTProg     = 3                       ///< CGSched: SMT Programmatic Scheduling
}CGSched;                             	///< CoreGenCore: SMT Scheduling Types

class CoreGenCore : public CoreGenNode{
private:
  CoreGenCache *Cache;                  ///< CoreGenCore: First connected cache hierarchy
  CoreGenISA *ISA;                      ///< CoreGenCore: ISA Implementation
  unsigned ThreadUnits;                 ///< CoreGenCore: SMT Thread Units
  CGSched Sched;			///< CoreGenCore: SMT Scheduler

  std::vector<CoreGenRegClass *> Regs;  ///< CoreGenCore: Supported registers
  std::vector<CoreGenNode *> Exts;      ///< CoreGenCore: Supported extensions

public:
  /// Default Constructor
  CoreGenCore(std::string Name, CoreGenISA *ISA, CoreGenErrno *E);

  /// Default Destructor
  ~CoreGenCore();

  /// Set the instruction set
  bool SetISA( CoreGenISA *ISA );

  /// Set the scheduler type
  bool SetSched( CGSched Sched );

  /// Add a cache hierarchy
  bool InsertCache( CoreGenCache *Cache );

  /// Add a register class
  bool InsertRegClass( CoreGenRegClass *RegClass );

  /// Add an extension
  bool InsertExt( CoreGenNode *Ext );

  /// Sets the number of thread units
  bool SetNumThreadUnits( unsigned TU );

  /// Retrieve the cache hierarchy
  CoreGenCache *GetCache() { return Cache; }

  /// Retrieve the ISA type
  CoreGenISA *GetISA() { return ISA; }

  /// Retrieve the number of implemented register classes
  unsigned GetNumRegClass() { return Regs.size(); }

  /// Retrieve the number of implemented extensions
  unsigned GetNumExt() { return Exts.size(); }

  /// Retrieve the number of thread units
  unsigned GetNumThreadUnits() { return ThreadUnits; }

  /// Retrieve the scheduler type
  CGSched GetSched() { return Sched; }

  /// Retrieve the target register class
  CoreGenRegClass *GetRegClass( unsigned C );

  /// Deletes the target register class
  bool DeleteRegClass( unsigned C );

  /// Deletes the target Extension
  bool DeleteExt( unsigned C );

  /// Retreive the target extension
  CoreGenNode *GetExt( unsigned E );

  /// Sets the cache to null
  bool SetNullCache() { Cache = nullptr; return true; }

  /// Sets the ISA to null
  bool SetNullISA() { ISA = nullptr; return true; }

  /// Converts scheduler type to string
  std::string CGSchedToStr( CGSched S );

  /// Converts a string to scheduler type
  CGSched StrToCGSched( std::string Str );
};

#endif

// EOF
