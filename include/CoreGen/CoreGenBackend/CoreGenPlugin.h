//
// _CoreGenPlugin_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenPlugin
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Pluginunication Channel Handler
 *
 */

#ifndef _COREGENPLUGIN_H_
#define _COREGENPLUGIN_H_

#include <iostream>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenPluginTypes.h"
#include "CoreGen/CoreGenBackend/CoreGenCache.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenPseudoInst.h"
#include "CoreGen/CoreGenBackend/CoreGenReg.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"
#include "CoreGen/CoreGenBackend/CoreGenComm.h"
#include "CoreGen/CoreGenBackend/CoreGenSpad.h"
#include "CoreGen/CoreGenBackend/CoreGenMCtrl.h"
#include "CoreGen/CoreGenBackend/CoreGenCore.h"

class CoreGenPlugin : public CoreGenNode{
private:
  CGPluginFunc Funcs;       ///< CoreGenPlugin: Function pointer structure
  CoreGenPluginImpl *Impl;  ///< CoreGenPlugin: Implementation object

public:

  /// Default constructor
  CoreGenPlugin(std::string Name, CGPluginFunc Funcs, CoreGenErrno *Errno);

  /// Defeault destructor
  ~CoreGenPlugin();

  /// Retrieve the number of features
  unsigned GetNumFeatures();

  /// Retrieve whether this plugin has an HDL Codegen
  bool HasHDLCodegen();

  /// Retreive whether this plugin has an LLVM Codegen
  bool HasLLVMCodegen();

  /// Retrieve the version info
  void GetVersion( unsigned *Major, unsigned *Minor, unsigned *Patch );

  /// Retrieve the function info
  CGPluginFunc GetFunctionInfo();

  /// Clone a plugin into a new node object
  CoreGenPlugin *ClonePlugin(std::string NewName);

  /// Retrieve the feature type of the target entity
  CGFeatureType GetFeatureType( std::string Feature );

  /// Retrieve the feature type of the target entity
  CGFeatureType GetFeatureType( unsigned Idx );

  /// Retrieve the feature name of the target entity
  std::string GetFeatureName( unsigned Idx );

  /// Retrieve the feature value
  CGFeatureVal GetFeatureValue( std::string Feature );

  /// Retrieve the feature value
  CGFeatureVal GetFeatureValue( unsigned Idx );

  /// Retrieve the top level plugin node
  CoreGenNode *GetTop();

  /// Set the feature value
  bool SetFeatureValue( std::string Feature, CGFeatureVal Val );

  /// Set the feature value
  bool SetFeatureValue( unsigned Idx, CGFeatureVal Val );

  /// Execute the HDL Codegen
  bool ExecuteHDLCodegen();

  /// Execute the LLVM Codegen
  bool ExecuteLLVMCodegen();

  /// Build the Plugin DAG
  bool BuildDAG();

  /// Retrieve the plugin name
  std::string GetPluginName();

  /// Insert a new core node
  CoreGenCore *InsertCore( std::string Name,
                           CoreGenISA *ISA );

  /// Insert a new ext node
  CoreGenExt *InsertExt( std::string Name );

  /// Insert a new comm node
  CoreGenComm *InsertComm( std::string Name );

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

  /// Insert a new cache node
  CoreGenCache *InsertCache(std::string Name, unsigned Sets,unsigned Ways);

  /// Retrieve the cache data
  std::vector<CoreGenCache *> *GetCaches();

  /// Retrieve the core data
  std::vector<CoreGenCore *> *GetCores();

  /// Retrieve the instruction data
  std::vector<CoreGenInst *> *GetInsts();

  /// Retreive the pseudo instruction data
  std::vector<CoreGenPseudoInst *> *GetPseudoInsts();

  /// Retrieve the instruction format data
  std::vector<CoreGenInstFormat *> *GetInstFormats();

  /// Retrieve the register data
  std::vector<CoreGenReg *> *GetRegs();

  /// Retrieve the register class data
  std::vector<CoreGenRegClass *> *GetRegClasses();

  /// Retrieve the SoC data
  std::vector<CoreGenSoC *> *GetSocs();

  /// Retrieve the ISA data
  std::vector<CoreGenISA *> *GetISAs();

  /// Retrieve the extension data
  std::vector<CoreGenExt *> *GetExts();

  /// Retrieve the communcation channel data
  std::vector<CoreGenComm *> *GetComms();

  /// Retrieve the scratchpad data
  std::vector<CoreGenSpad *> *GetSpads();

  /// Retrieve the memory controller data
  std::vector<CoreGenMCtrl *> *GetMCtrls();

  /// Retrieve the virtual to physical translation data
  std::vector<CoreGenVTP *> *GetVTPs();

  /// Retrieve the number of caches
  unsigned GetNumCaches();

  /// Retrieve the number of cores
  unsigned GetNumCores();

  /// Retrieve the number instructions
  unsigned GetNumInsts();

  /// Retrieve the number of psuedo instructions
  unsigned GetNumPseudoInsts();

  /// Retrieve the number of instruction formats
  unsigned GetNumFormats();

  /// Retrieve the number of registers
  unsigned GetNumRegs();

  /// Retrieve the number of register classes
  unsigned GetNumRegClasses();

  /// Retrieve the number of SoCs
  unsigned GetNumSocs();

  /// Retrieve the number ISAs
  unsigned GetNumISAs();

  /// Retrieve the number of extensions
  unsigned GetNumExts();

  /// Retrieve the number of communication channels
  unsigned GetNumComms();

  /// Retrieve the number of scratchpads
  unsigned GetNumSpads();

  /// Retrieve the number of memory controllers
  unsigned GetNumMCtrls();

  /// Retrieve the number of virtual to physical units
  unsigned GetNumVTPs();

  /// Retrieve a reference to the cache vector
  std::vector<CoreGenCache *> &GetCacheVect();

  /// Retrieve a reference to the insts vector
  std::vector<CoreGenInst *> &GetInstVect();

  /// Retrieve a reference to the exts vector
  std::vector<CoreGenExt *> &GetExtVect();

  /// Retrieve a reference to the pseudo insts vector
  std::vector<CoreGenPseudoInst *> &GetPseudoInstVect();

  /// Retrieve a reference to the instruction format vector
  std::vector<CoreGenInstFormat *> &GetInstFormatVect();

  /// Retrieve a reference to the register vector
  std::vector<CoreGenReg *> &GetRegVect();

  /// Retreive a reference to the register class vector
  std::vector<CoreGenRegClass *> &GetRegClassVect();

  /// Retrieve a reference to the ISAs vector
  std::vector<CoreGenISA *> &GetISAVect();

  /// Retrieve a reference to the comms vector
  std::vector<CoreGenComm *> &GetCommVect();

  /// Retrieve a reference to the cores vector
  std::vector<CoreGenCore *> &GetCoreVect();

  /// Retrieve a reference to the scratchpads vector
  std::vector<CoreGenSpad *> &GetSpadVect();

  /// Retrieve a reference to the memory controllers vector
  std::vector<CoreGenMCtrl *> &GetMCtrlVect();

  /// Retrieve a reference to the virtual to physical vector
  std::vector<CoreGenVTP *> &GetVTPVect();

  /// Retrieves a reference to the soc vector
  std::vector<CoreGenSoC *> &GetSocVect();
};

#endif

// EOF
