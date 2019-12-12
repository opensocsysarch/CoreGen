//
// _CoreGenPluginImpl_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/** \defgroup CoreGenPlugin CoreGen High Level System on Chip (SoC) Design Framework Plugin Infrastructure
 *
 * \brief The CoreGenPlugin library provides an infrastructure to construct CoreGen 
 *        plugin shared libraries for user-specific and language-specific functionality
 *
 */

/**
 * \class CoreGenPluginImpl
 *
 * \ingroup CoreGenPlugin
 *
 * \brief CoreGen Plugin Implementation Base Class
 *
 */

#ifndef _COREGENPLUGINIMPL_H_
#define _COREGENPLUGINIMPL_H_

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
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

/*! \enum CGPluginType
 *  \brief Defnes the type of the CoreGenPlugin
 *
 * Defines the supported types of plugins in the
 * CoreGen infrastructure
 *
 */
typedef enum{
  CGPNode         = 0,    ///< CGPluginType: Node override plugin
  CGPTemplate     = 1     ///< CGPluginType: Template plugin
}CGPluginType;

/*! \enum CGFeatureType
 *  \brief Defines the CoreGenPlugin feature types
 *
 *  Defines the different styles of CoreGenPlugin features
 *  that individual plugins may utilize for custom extensions
 *  to the standard CoreGenNode types
 *
 */
typedef enum{
  CGFUnsigned     = 0,    ///< CGFeatureType: unsigned integer
  CGFUin32t       = 1,    ///< CGFeatureType: uint32_t
  CGFInt32t       = 2,    ///< CGFeatureType: int32_t
  CGFUint64t      = 3,    ///< CGFeatureType: uint64_t
  CGFInt64t       = 4,    ///< CGFeatureType: in64_t
  CGFFloat        = 5,    ///< CGFeatureType: Float
  CGFDouble       = 6,    ///< CGFeatureType: Double
  CGFString       = 7,    ///< CGFeatureType: String
  CGFBool         = 8,    ///< CGFeatureType: Bool
  CGFUnknown      = 100   ///< CGFeatureType: Unknown
}CGFeatureType;

/*! \struct CGFeatureVal
 *  \brief Defines the values behind each new feature
 *
 *  Defines individual feature values for a single
 *  CGFeatureType entry
 *
 */
typedef struct{
  unsigned UnsignedData   = 0;      ///< CGFeatureVal: unsigned int data
  uint32_t Uint32tData    = 0;      ///< CGFeatureVal: uint32_t data
  int32_t Int32tData      = 0;      ///< CGFeatureVal: int32_t data
  uint64_t Uint64tData    = 0;      ///< CGFeatureVal: uint64_t data
  int64_t Int64tData      = 0;      ///< CGFeatureVal: int64_t data
  float FloatData         = 0.;     ///< CGFeatureVal: float data
  double DoubleData       = 0.;     ///< CGFeatureVal: double data
  std::string StringData  = "";     ///< CGFeatureVal: string data
  bool BoolData           = false;  ///< CGFeatureVal: bool data
}CGFeatureVal;

/*! \struct CGFeatureTable
 *  \brief Defines a single plugin feature table entry
 *
 *  Defines a single plugin feature table entry
 *
 */
typedef struct{
  std::string Name;       ///< CGFeatureTable: Feature Name
  CGFeatureType Type;     ///< CGFeatureTable: Feature Type
  CGFeatureVal Value;     ///< CGFeatureTable: Feature Value
}CGFeatureTable;

/**
 * \class CoreGenPluginImpl
 *
 * \ingroup CoreGen
 *
 * \brief Defines the base class for CoreGenPlugin implementations
 *
 */
class CoreGenPluginImpl{
private:
  std::string Name;       ///< CoreGenPluginImpl: Plugin Name
  CGPluginType Type;      ///< CoreGenPluginImpl: Plugin Type
  unsigned NumFeatures;   ///< CoreGenPluginImpl: Number of features
  bool HDLCodegen;        ///< CoreGenPluginImpl: Is there an HDL Codegen
  bool LLVMCodegen;       ///< CoreGenPluginImpl: Is there an LLVM Codegen
  unsigned MajorVersion;  ///< CoreGenPluginImpl: Major Version
  unsigned MinorVersion;  ///< CoreGenPluginImpl: Minor Version
  unsigned PatchVersion;  ///< CoreGenPluginImpl: Patch Version

  std::vector<CGFeatureTable> FeatureTable; ///< CoreGenPluginImpl: Extended Feature Table

  std::vector<CoreGenNode *> Nodes;         ///< CoreGenPluginImpl: DAG nodes

  std::vector<CoreGenCache *> Caches;       ///< CoreGenImpl: Caches
  std::vector<CoreGenCore *> Cores;         ///< CoreGenImpl: Cores
  std::vector<CoreGenInst *> Insts;         ///< CoreGenImpl: Instructions
  std::vector<CoreGenPseudoInst *> PInsts;  ///< CoreGenImpl: Pseudo Instructions
  std::vector<CoreGenInstFormat *> Formats; ///< CoreGenImpl: Instruction Formats
  std::vector<CoreGenReg *> Regs;           ///< CoreGenImpl: Registers
  std::vector<CoreGenRegClass *> RegClasses;///< CoreGenImpl: Register Classes
  std::vector<CoreGenSoC *> Socs;           ///< CoreGenImpl: SoC's
  std::vector<CoreGenISA *> ISAs;           ///< CoreGenImpl: ISA's
  std::vector<CoreGenExt *> Exts;           ///< CoreGenImpl: Extensions
  std::vector<CoreGenComm *> Comms;         ///< CoreGenImpl: Communication channels
  std::vector<CoreGenSpad *> Spads;         ///< CoreGenImpl: Scratchpads
  std::vector<CoreGenMCtrl *> MCtrls;       ///< CoreGenImpl: Memory Controllers
  std::vector<CoreGenVTP *> VTPs;           ///< CoreGenImpl: Virtual to Physical Controllers

protected:
  CoreGenErrno *Errno;    ///< CoreGenPluginImpl: Errno structure
  CoreGenNode *Top;       ///< CoreGenPluginImpl: Top-level component node


public:
  /// Default Constructor
  CoreGenPluginImpl(std::string Name,
                    CGPluginType Type,
                    bool HDL,
                    bool LLVM,
                    unsigned MajorVersion,
                    unsigned MinorVersion,
                    unsigned PatchVersion,
                    CoreGenErrno *E);

  /// Retrieve the cache data
  std::vector<CoreGenCache *> *GetCaches() { return &Caches; }

  /// Retrieve the core data
  std::vector<CoreGenCore *> *GetCores() { return &Cores; }

  /// Retrieve the instruction data
  std::vector<CoreGenInst *> *GetInsts() { return &Insts; }

  /// Retreive the pseudo instruction data
  std::vector<CoreGenPseudoInst *> *GetPseudoInsts() { return &PInsts; }

  /// Retrieve the instruction format data
  std::vector<CoreGenInstFormat *> *GetInstFormats() { return &Formats; }

  /// Retrieve the register data
  std::vector<CoreGenReg *> *GetRegs() { return &Regs; }

  /// Retrieve the register class data
  std::vector<CoreGenRegClass *> *GetRegClasses() { return &RegClasses; }

  /// Retrieve the SoC data
  std::vector<CoreGenSoC *> *GetSocs() { return &Socs; }

  /// Retrieve the ISA data
  std::vector<CoreGenISA *> *GetISAs() { return &ISAs; }

  /// Retrieve the extension data
  std::vector<CoreGenExt *> *GetExts() { return &Exts; }

  /// Retrieve the communcation channel data
  std::vector<CoreGenComm *> *GetComms() { return &Comms; }

  /// Retrieve the scratchpad data
  std::vector<CoreGenSpad *> *GetSpads() { return &Spads; }

  /// Retrieve the memory controller data
  std::vector<CoreGenMCtrl *> *GetMCtrls() { return &MCtrls; }

  /// Retrieve the virtual to physical translation data
  std::vector<CoreGenVTP *> *GetVTPs() { return &VTPs; }

  /// Retrieve the number of caches
  unsigned GetNumCaches() { return VTPs.size(); }

  /// Retrieve the number of cores
  unsigned GetNumCores(){ return Cores.size(); }

  /// Retrieve the number instructions
  unsigned GetNumInsts() { return Insts.size(); }

  /// Retrieve the number of psuedo instructions
  unsigned GetNumPseudoInsts() { return PInsts.size(); }

  /// Retrieve the number of instruction formats
  unsigned GetNumFormats() { return Formats.size(); }

  /// Retrieve the number of registers
  unsigned GetNumRegs() { return Regs.size(); }

  /// Retrieve the number of register classes
  unsigned GetNumRegClasses() { return RegClasses.size(); }

  /// Retrieve the number of SoCs
  unsigned GetNumSocs() { return Socs.size(); }

  /// Retrieve the number ISAs
  unsigned GetNumISAs() { return ISAs.size(); }

  /// Retrieve the number of extensions
  unsigned GetNumExts() { return Exts.size(); }

  /// Retrieve the number of communication channels
  unsigned GetNumComms() { return Comms.size(); }

  /// Retrieve the number of scratchpads
  unsigned GetNumSpads() { return Spads.size(); }

  /// Retrieve the number of memory controllers
  unsigned GetNumMCtrls() { return MCtrls.size(); }

  /// Retrieve the number of virtual to physical units
  unsigned GetNumVTPs() { return VTPs.size(); }

  /// Retrieve the plugin name
  std::string GetName() { return Name; }

  /// Retrieve the number of features
  unsigned GetNumFeatures() { return NumFeatures; }

  /// Retrieve whether this plugin has an HDL Codegen
  bool HasHDLCodegen() { return HDLCodegen; }

  /// Retreive whether this plugin has an LLVM Codegen
  bool HasLLVMCodegen() { return LLVMCodegen; }

  /// Retrieve the version info
  void GetVersion( unsigned *Major, unsigned *Minor, unsigned *Patch );

  /// Insert a feature into the feature table
  bool InsertFeature(std::string Name,
                     CGFeatureType Type,
                     CGFeatureVal Value);

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

  /// Retrieve the top-level node
  CoreGenNode *GetTop() { return Top; }

  /// Retrieve the errno structure
  CoreGenErrno *GetErrno() { return Errno; }

  /// Initializes the feature value structure
  void InitFeatureValue(CGFeatureVal &Value);

  /// Set the feature value
  bool SetFeatureValue( std::string Feature, CGFeatureVal Val );

  /// Set the feature value
  bool SetFeatureValue( unsigned Idx, CGFeatureVal Val );

  /// Insert a new DAG node
  bool InsertNode( CoreGenNode *N );

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

  /// Build the DAG for executing passes
  bool BuildDAG();

  /// Purge all the child node mappings
  bool PurgeNodes();

  /// Retrieve a reference to the cache vector
  std::vector<CoreGenCache *> &GetCacheVect(){ return Caches; }

  /// Retrieve a reference to the insts vector
  std::vector<CoreGenInst *> &GetInstVect() { return Insts; }

  /// Retrieve a reference to the exts vector
  std::vector<CoreGenExt *> &GetExtVect() { return Exts; }

  /// Retrieve a reference to the pseudo insts vector
  std::vector<CoreGenPseudoInst *> &GetPseudoInstVect() { return PInsts; }

  /// Retrieve a reference to the instruction format vector
  std::vector<CoreGenInstFormat *> &GetInstFormatVect() { return Formats; }

  /// Retrieve a reference to the register vector
  std::vector<CoreGenReg *> &GetRegVect() { return Regs; }

  /// Retreive a reference to the register class vector
  std::vector<CoreGenRegClass *> &GetRegClassVect() { return RegClasses; }

  /// Retrieve a reference to the ISAs vector
  std::vector<CoreGenISA *> &GetISAVect() { return ISAs; }

  /// Retrieve a reference to the comms vector
  std::vector<CoreGenComm *> &GetCommVect() { return Comms; }

  /// Retrieve a reference to the cores vector
  std::vector<CoreGenCore *> &GetCoreVect() { return Cores; }

  /// Retrieve a reference to the scratchpads vector
  std::vector<CoreGenSpad *> &GetSpadVect() { return Spads; }

  /// Retrieve a reference to the memory controllers vector
  std::vector<CoreGenMCtrl *> &GetMCtrlVect() { return MCtrls; }

  /// Retrieve a reference to the virtual to physical vector
  std::vector<CoreGenVTP *> &GetVTPVect() { return VTPs; }

  /// Retrieve a reference to the soc vector
  std::vector<CoreGenSoC *> &GetSocVect() { return Socs; }

  /// Retrieve the type of the target plugin
  CGPluginType GetPluginType();

  // -- virtual methods --

  /// Process the plugin features
  virtual bool ProcessFeatures() { return false; }

  /// Execute the HDL Codegen
  virtual bool ExecuteHDLCodegen() { return false; }

  /// Execute the LLVM Codegen
  virtual bool ExecuteLLVMCodegen() { return false; }

  /// Default Destructor
  virtual ~CoreGenPluginImpl();
};

#endif
// EOF
