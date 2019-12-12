//
// _CoreGenPluginMgr_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenPluginMgr
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Plugin Management Framework
 *
 */

#ifndef _COREGENPLUGINMGR_H_
#define _COREGENPLUGINMGR_H_

#include <iostream>
#include <string>
#include <vector>

#if (COREGEN_PLATFORM == Darwin) || (COREGEN_PLATFORM == Linux)

// Unix headers
#include <dlfcn.h>

#else
// Win64 headers
#endif

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenPlugin.h"

class CoreGenPluginMgr{
private:
  CoreGenErrno *Errno;                    ///< CoreGenPluginMgr: Errno handler
  std::vector<CoreGenPlugin *> Plugins;   ///< CoreGenPluginMgr: Vector of loaded plugins
  std::vector<CGPluginFunc> FuncPtrs;     ///< CoreGenPluginMgr: Vector of plugin function pointers

#if (COREGEN_PLATFORM == Darwin) || (COREGEN_PLATFORM == Linux)
  // Unix-specific plugin data

  std::vector<void *> Handles;            ///< CoreGenPluginMgr: Dynamic Loader handles

  /// Unix plugin loader
  bool LoadNixPlugin( std::string Path );
#else
  // Windows-specific plugin data
#endif


public:

  /// Default constructor
  CoreGenPluginMgr(CoreGenErrno *E);

  /// Default destructor
  ~CoreGenPluginMgr();

  /// Load a plugin
  bool LoadPlugin( std::string Path );

  /// Release a plugin
  bool ReleasePlugin( unsigned Idx );

  /// Release a plugin
  bool ReleasePlugin( std::string Plugin);

  /// Release a plugin
  bool ReleasePlugin( std::string Plugin,
                      unsigned Major,
                      unsigned Minor,
                      unsigned Patch );

  /// Retrieve the number of loaded plugins
  unsigned GetNumPlugins() { return Plugins.size(); }

  /// Retrieve a pointer to the target plugin
  CoreGenPlugin *GetPlugin( unsigned Plugin );

  /// Retreive a pointer to the target plugin
  CoreGenPlugin *GetPlugin( std::string Plugin );

  /// Retrieve a pointer to the target plugin
  CoreGenPlugin *GetPlugin( std::string Plugin,
                            unsigned Major,
                            unsigned Minor,
                            unsigned Patch );

};

#endif

// EOF
