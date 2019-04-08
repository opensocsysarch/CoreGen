//
// _CoreGenBackend_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/** \defgroup CoreGen CoreGen High Level System on Chip (SoC) Design Framework
 *
 * \brief The CoreGen library provides a high-level design and dependence
 *        analysis framework for homogeneous and heterogenous system on chips.
 */


/**
 * \class CoreGenBackend
 *
 * \ingroup CoreGen
 *
 * \brief The top-level CoreGen class infrastructure
 *
 */

#ifndef _COREGENBACKEND_H_
#define _COREGENBACKEND_H_

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <vector>

// Infrastructure Headers
#include "CoreGen/CoreGenBackend/CoreGenVersion.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenEnv.h"
#include "CoreGen/CoreGenBackend/CoreGenProj.h"

#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenPluginMgr.h"
#include "CoreGen/CoreGenBackend/CoreGenPassMgr.h"
#include "CoreGen/CoreGenBackend/CoreGenYaml.h"

// Node Headers
#include "CoreGen/CoreGenBackend/CoreGenCache.h"
#include "CoreGen/CoreGenBackend/CoreGenCore.h"
#include "CoreGen/CoreGenBackend/CoreGenExt.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenPseudoInst.h"
#include "CoreGen/CoreGenBackend/CoreGenReg.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"
#include "CoreGen/CoreGenBackend/CoreGenSoC.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"
#include "CoreGen/CoreGenBackend/CoreGenComm.h"
#include "CoreGen/CoreGenBackend/CoreGenSpad.h"
#include "CoreGen/CoreGenBackend/CoreGenMCtrl.h"
#include "CoreGen/CoreGenBackend/CoreGenVTP.h"
#include "CoreGen/CoreGenBackend/CoreGenPlugin.h"

// Codegen Headers
#include "CoreGen/CoreGenCodegen/CoreGenCodegen.h"


class CoreGenBackend
{
private:
  CoreGenErrno *Errno;                      ///< CoreGenBackend: Errno
  CoreGenProj *Proj;                        ///< CoreGenBackend: Project Handler
  CoreGenEnv *Env;                          ///< CoreGenBackend: Execution environment

  CoreGenPluginMgr *PluginMgr;              ///< CoreGenBackend: Plugin Manager

  CoreGenPassMgr *PassMgr;                  ///< CoreGenBackend: Pass Manager

  std::vector<CoreGenCache *> Caches;       ///< CoreGenBackend: Caches
  std::vector<CoreGenCore *> Cores;         ///< CoreGenBackend: Cores
  std::vector<CoreGenInst *> Insts;         ///< CoreGenBackend: Instructions
  std::vector<CoreGenPseudoInst *> PInsts;  ///< CoreGenBackend: Pseudo Instructions
  std::vector<CoreGenInstFormat *> Formats; ///< CoreGenBackend: Instruction Formats
  std::vector<CoreGenReg *> Regs;           ///< CoreGenBackend: Registers
  std::vector<CoreGenRegClass *> RegClasses;///< CoreGenBackend: Register Classes
  std::vector<CoreGenSoC *> Socs;           ///< CoreGenBackend: SoC's
  std::vector<CoreGenISA *> ISAs;           ///< CoreGenBackend: ISA's
  std::vector<CoreGenExt *> Exts;           ///< CoreGenBackend: Extensions
  std::vector<CoreGenComm *> Comms;         ///< CoreGenBackend: Communication channels
  std::vector<CoreGenSpad *> Spads;         ///< CoreGenBackend: Scratchpads
  std::vector<CoreGenMCtrl *> MCtrls;       ///< CoreGenBackend: Memory Controllers
  std::vector<CoreGenVTP *> VTPs;           ///< CoreGenBackend: Virtual to Physical Controllers
  std::vector<CoreGenPlugin *> Plugins;     ///< CoreGenBackend: Plugins

  CoreGenNode *Top;                         ///< CoreGenBackend: Top-level project node

  CoreGenDAG *DAG;                          ///< CoreGeBackend: Highest DAG node

public:

  /// Default Constructor
  CoreGenBackend();

  /// Overloaded Constructor
  CoreGenBackend( std::string ProjName,
                  std::string ProjRoot,
                  std::string ArchRoot );

  /// Overloaded Constructor
  CoreGenBackend( std::string ProjName,
                  std::string ProjRoot,
                  std::string ArchRoot,
                  CGProjType Type );

  /// Default Destructor
  ~CoreGenBackend();

  /// Retrieve Version Information
  void CoreGenVersion( int *major, int *minor );

  /// Write the Yaml IR out to a file
  bool WriteIR( std::string FileName );

  /// Read the Yaml IR in from a file
  bool ReadIR( std::string FileName );

  /// Load a plugin
  bool LoadPlugin( std::string Path );

  /// Release the target plugin
  bool ReleasePlugin( unsigned Idx );

  /// Release the target plugin
  bool ReleasePlugin( std::string Plugin );

  /// Retrieve the number of loaded plugins
  unsigned GetNumPlugins();

  /// Retrieve the error string
  std::string GetErrStr();

  /// Insert a new cache node
  CoreGenCache *InsertCache(std::string Name,
                          unsigned Sets,
                          unsigned Ways);

  /// Insert a new core node
  CoreGenCore *InsertCore( std::string Name,
                           CoreGenISA *ISA );

  /// Insert a new ext node
  CoreGenExt *InsertExt( std::string Name );

  /// Insert a new comm node
  CoreGenComm *InsertComm( std::string Name );

  /// Insert a new scratchpad node
  CoreGenSpad *InsertSpad( std::string Name,
                           unsigned MemSize,
                           unsigned RqstPorts,
                           unsigned RspPorts );

  /// Insert a new memory controller node
  CoreGenMCtrl *InsertMCtrl( std::string Name,
                             unsigned Ports );

  /// Insert a new inst node
  CoreGenInst *InsertInst( std::string Name,
                           CoreGenISA *ISA,
                           CoreGenInstFormat *Format );

  /// Insert a new pseudo instruction
  CoreGenPseudoInst *InsertPseudoInst( std::string Name,
                                       CoreGenInst *Inst );

  /// Insert a new instruction format
  CoreGenInstFormat *InsertInstFormat( std::string Name,
                                       CoreGenISA *ISA );

  /// Insert a new register node
  CoreGenReg *InsertReg( std::string Name, int RegIndex, int RegWidth );

  /// Insert a new register class node
  CoreGenRegClass *InsertRegClass( std::string Name );

  /// Insert a new SoC node
  CoreGenSoC *InsertSoC( std::string Name );

  /// Insert a new ISA node
  CoreGenISA *InsertISA( std::string Name );

  /// Insert a new VTP node
  CoreGenVTP *InsertVTP( std::string Name );

  /// Insert a loaded plugin node
  bool InsertPlugin( CoreGenPlugin *Plugin );

  /// Build the DAG for executing passes
  bool BuildDAG();

  /// Purge all the top-level nodes, but do not delete them
  bool PurgeDAG();

  /// Export the DAG in a DOT graphviz file
  bool ExportDOT(std::string FName);

  /// Determines whether the pass manage is initialized
  bool IsPassMgr();

  /// Initialize the pass manager
  bool InitPassMgr();

  /// Initialize the pass manager
  bool InitPassMgr(std::ostream *O);

  /// Retrieve a list of all the passes
  std::vector<std::string> GetPasses();

  /// Print all the pass info
  bool PrintPassInfo();

  /// Print all the system pass info
  bool PrintSysPassInfo();

  /// Execute All Passes
  bool ExecutePasses();

  /// Execute a specific pass
  bool ExecutePass( std::string Pass );

  /// Execute a class of passes
  bool ExecutePassType(CGPassType T );

  /// Execute a specific system pass
  bool ExecuteSysPass( std::string Pass );

  /// Set the output path for the target system pass
  bool SetPassOutputPath( std::string Pass, std::string Path );

  /// Set the input string for the target system pass
  bool SetPassInputStr( std::string Pass, std::string Str );

  /// Execute the codegen
  bool ExecuteCodegen();

  /// Execute the LLVM codegen
  bool ExecuteLLVMCodegen();

  /// Execute the Chisel codegen
  bool ExecuteChiselCodegen();

  /// Retrieve the build date
  std::string CoreGenBuildDate() { return CGBuildDate(); }

  /// Retrieve the build time
  std::string CoreGenBuildTime() { return CGBuildTime(); }

  /// Retrieve an object pointer to the target plugin
  CoreGenPlugin *GetPlugin( unsigned Plugin );

  /// Retrieve an object pointer to the target plugin
  CoreGenPlugin *GetPlugin( std::string Plugin );

  /// Set the ASP Files to Test
  void SetASPFiles(std::vector<std::string>);
};

#endif

// EOF
