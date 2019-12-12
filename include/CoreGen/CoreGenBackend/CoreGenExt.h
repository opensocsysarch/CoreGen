//
// _CoreGenExt_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenExt
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Extension Node Handler
 *
 */

#ifndef _COREGENEXT_H_
#define _COREGENEXT_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenProj.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
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

typedef enum{
  CGExtTemplate = 1,                        ///< CGExtType: Template extension
  CGExtModule   = 2,                        ///< CGExtType: Module extension
  CGExtComm     = 3,                        ///< CGExtType: Comm extension
  CGExtUnk      = 100                       ///< CGExtType: Unknown extension type
}CGExtType;                                 ///< CoreGenExt: Extension types

class CoreGenExt : public CoreGenNode{
private:
  CGExtType Type;                           ///< CoreGenExt: Extension type

  std::vector<CoreGenCache *> Caches;       ///< CoreGenExt: Caches
  std::vector<CoreGenExt *> Exts;           ///< CoreGenExt: Extensions
  std::vector<CoreGenInst *> Insts;         ///< CoreGenExt: Instructions
  std::vector<CoreGenPseudoInst *> PInsts;  ///< CoreGenExt: Pseudo Instructions
  std::vector<CoreGenInstFormat *> Formats; ///< CoreGenExt: Instruction Formats
  std::vector<CoreGenReg *> Regs;           ///< CoreGenExt: Registers
  std::vector<CoreGenRegClass *> RegClasses;///< CoreGenExt: Register Classes
  std::vector<CoreGenISA *> ISAs;           ///< CoreGenExt: ISA's
  std::vector<CoreGenComm *> Comms;         ///< CoreGenExt: Communication channels
  std::vector<CoreGenCore *> Cores;         ///< CoreGenExt: Cores
  std::vector<CoreGenSpad *> Spads;         ///< CoreGenExt: Scratchpads
  std::vector<CoreGenMCtrl *> MCtrls;       ///< CoreGenExt: Memory Controllers

public:

  /// Default constructor
  CoreGenExt( std::string Name,
              CoreGenErrno *E );

  // Default Destructor
  ~CoreGenExt();

  /// Retrieve the extension type
  CGExtType GetType() { return Type; }

  /// Set the extension type
  bool SetType( CGExtType T );

  /// Retrieve the caches data
  std::vector<CoreGenCache *> *GetCaches(){ return &Caches; }

  /// Retrieve the extensions data
  std::vector<CoreGenExt *> *GetExts() { return &Exts; }

  /// Retrieve the instruction data
  std::vector<CoreGenInst *> *GetInsts() { return &Insts; }

  /// Retrieve the pseudo instruction data
  std::vector<CoreGenPseudoInst *> *GetPseudoInsts() { return &PInsts; }

  /// Retrieve the instruction format data
  std::vector<CoreGenInstFormat *> *GetInstFormats() { return &Formats; }

  /// Retrieve the registers
  std::vector<CoreGenReg *> *GetRegs() { return &Regs; }

  /// Retrieve the register classes
  std::vector<CoreGenRegClass *> *GetRegClasses() { return &RegClasses; }

  /// Retrieve the instruction sets
  std::vector<CoreGenISA *> *GetISAs() { return &ISAs; }

  /// Retrieve the Comms
  std::vector<CoreGenComm *> *GetComms() { return &Comms; }

  /// Retrieve the Scratchapds
  std::vector<CoreGenSpad *> *GetSpads() { return &Spads; }

  /// Retrieve the Memory Controllers
  std::vector<CoreGenMCtrl *> *GetMCtrls() { return &MCtrls; }

  /// Retrieve the cores
  std::vector<CoreGenCore *> *GetCores() { return &Cores; }

  /// Retrieve the number of child Cache nodes
  unsigned GetNumCaches() { return Caches.size(); }

  /// Retrieve the number of child Ext nodes
  unsigned GetNumExts() { return Exts.size(); }

  /// Retrieve the number of child Inst nodes
  unsigned GetNumInsts() { return Insts.size(); }

  /// Retrieve the number of Pseudo Inst nodes
  unsigned GetNumPseudoInsts() { return PInsts.size(); }

  /// Retrieve the number of child Format nodes
  unsigned GetNumFormats() { return Formats.size(); }

  /// Retrieve the number of child Reg nodes
  unsigned GetNumRegs() { return Regs.size(); }

  /// Retrieve the number of child RegClass nodes
  unsigned GetNumRegClasses() { return RegClasses.size(); }

  /// Retrieve the number of child ISA nodes
  unsigned GetNumISAs() { return ISAs.size(); }

  /// Retrieve the number child cores
  unsigned GetNumCores() { return Cores.size(); }

  /// Retrieve the number of child comms
  unsigned GetNumComms() { return Comms.size(); }

  /// Retrieve the number of child scratchpads
  unsigned GetNumSpads() { return Spads.size(); }

  /// Retrieve the number of child memory controllers
  unsigned GetNumMCtrls() { return MCtrls.size(); }

  /// Updates the child node structure after inserting new nodes
  bool UpdateChildNodes();

  /// Insert a new cache node
  CoreGenCache *InsertCache(std::string Name,
                          unsigned Sets,
                          unsigned Ways);

  /// Insert an existing cache node
  bool InsertCache( CoreGenCache *C );

  /// Insert a new core node
  CoreGenCore *InsertCore( std::string Name,
                           CoreGenISA *ISA );

  /// Insert an exsting core node
  bool InsertCore( CoreGenCore *C );

  /// Insert a new comm node
  CoreGenComm *InsertComm( std::string Name );

  /// Insert an existing comm node
  bool InsertComm( CoreGenComm *C );

  /// Insert a new inst node
  CoreGenInst *InsertInst( std::string Name,
                           CoreGenISA *ISA,
                           CoreGenInstFormat *Format );

  /// Insert an existing inst node
  bool InsertInst( CoreGenInst *I );

  /// Insert a new pseudo instruction
  CoreGenPseudoInst *InsertPseudoInst( std::string Name,
                                       CoreGenInst *Inst );

  /// Insert an existing pseudo inst
  bool InsertPseudoInst( CoreGenPseudoInst *I );

  /// Insert a new instruction format
  CoreGenInstFormat *InsertInstFormat( std::string Name,
                                       CoreGenISA *ISA );

  /// Insert a new register node
  CoreGenReg *InsertReg( std::string Name, int RegIndex, int RegWidth );

  /// Insert an existing register node
  bool InsertReg( CoreGenReg *R );

  /// Insert a new register class node
  CoreGenRegClass *InsertRegClass( std::string Name );

  /// Insert an existing register class node
  bool InsertRegClass( CoreGenRegClass *R );

  /// Insert a new ISA node
  CoreGenISA *InsertISA( std::string Name );

  /// Insert an existing ISA node
  bool InsertISA( CoreGenISA *I );

  /// Insert an existing instruction format
  bool InsertInstFormat( CoreGenInstFormat *I );

  /// Insert an existing extension
  bool InsertExt( CoreGenExt *E );

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

};

#endif

// EOF
