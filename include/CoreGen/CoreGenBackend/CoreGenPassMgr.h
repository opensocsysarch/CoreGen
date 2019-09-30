//
// _CoreGenPassMgr_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenPassMgr
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Pass Manager
 *
 */

#ifndef _COREGENPASSMGR_H_
#define _COREGENPASSMGR_H_

// Standard Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <tuple>

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenProj.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/Passes/CoreGenPasses.h"

class CoreGenPassMgr {
private:
  std::ostream *Stream;         ///< CoreGenpassMgr: Output stream
  CoreGenErrno *Errno;          ///< CoreGenPassMgr: Errno handler
  CoreGenDAG *DAG;              ///< CoreGenPassMgr: Top-level DAG
  CoreGenProj *Proj;            ///< CoreGenPassMgr: Project handler

  std::vector<CoreGenPass *> Passes;    ///< CoreGenPassMgr: Pass vector

  std::vector<CoreGenPass *> SysPasses; ///< CoreGenPassMgr: System pass vector

  /// Initialize SoC Project passes
  void InitSoCPasses(std::ostream *O);

  /// Initialize Module Project passes
  void InitModPasses(std::ostream *O);

  /// Initialize Extension Project passes
  void InitExtPasses(std::ostream *O);

  /// Initialize System Passes
  void InitSysPasses(std::ostream *O);

public:

  /// Default constructor
  CoreGenPassMgr( std::ostream *O, CoreGenErrno *E,
                  CoreGenDAG *D, CoreGenProj *P );

  /// Default destructor
  ~CoreGenPassMgr();

  /// Execute all the passes
  bool Execute();

  /// Execute a specific pass
  bool Execute( std::string Pass );

  /// Execute a specific class of passes
  bool Execute( CGPassType T );

  /// Execute a specific system pass
  bool ExecuteSys( std::string Pass );

  /// Execute all the system passes
  bool ExecuteSys();

  /// Retrieve a list of all initialized pass names
  std::vector<std::string> GetPassNames();

  /// Retrieve a list of all initialized pass descriptions
  std::vector<std::string> GetPassDescriptions();

  /// Print all the pass information to the status stream
  bool PrintPassInfo();

  /// Print all the pass information for system passes to the status stream
  bool PrintSysPassInfo();

  /// Sets the output path for the target system pass
  bool SetPassOutputPath( std::string Pass, std::string Path );

  /// Sets the intput string for the target system pass
  bool SetPassInputStr( std::string Pass, std::string Str );

  /// Sets the ASP files
  void SetASPFiles(std::vector<std::string>);
};

#endif

// EOF
