//
// _CoreGenYaml_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREGENYAML_H_
#define _COREGENYAML_H_

#include <string>
#include <vector>
#include <fstream>

#include "yaml-cpp/yaml.h"

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenBackend/CoreGenEnv.h"

#include "CoreGen/CoreGenBackend/CoreGenPluginMgr.h"

#include "CoreGen/CoreGenBackend/CoreGenCache.h"
#include "CoreGen/CoreGenBackend/CoreGenCore.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"
#include "CoreGen/CoreGenBackend/CoreGenExt.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenPseudoInst.h"
#include "CoreGen/CoreGenBackend/CoreGenReg.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"
#include "CoreGen/CoreGenBackend/CoreGenSoC.h"
#include "CoreGen/CoreGenBackend/CoreGenComm.h"
#include "CoreGen/CoreGenBackend/CoreGenSpad.h"
#include "CoreGen/CoreGenBackend/CoreGenMCtrl.h"
#include "CoreGen/CoreGenBackend/CoreGenVTP.h"
#include "CoreGen/CoreGenBackend/CoreGenProj.h"

#include "CoreGen/CoreGenBackend/CoreGenDAG.h"


/**
 * \class CoreGenYaml
 *
 * \ingroup CoreGen
 *
 * \brief YAML reader and writer class
 *
 */


class CoreGenYaml{
private:
  std::string FileName;           ///< CoreGenYaml: YAML file name
  CoreGenErrno *Errno;            ///< CoreGenYaml: Errno handler
  unsigned ParserErrorLine;       ///< CoreGenYaml: Last parser error line number

  CoreGenPluginMgr *PluginMgr;    ///< CoreGenBackend: Plugin Manager

  CoreGenEnv *Env;                ///< CoreGenBackend: CoreGen environment
  CoreGenProj *Proj;              ///< CoreGenBackend: CoreGen project info

  std::vector<std::string> CNames; ///< CoreGenYaml: Cache printer storage

  std::vector<CoreGenPlugin *> *PluginPtr;  ///< CoreGenYaml: Pointer to plugin vector

  /// Determine if the YAML node name is valid
  bool IsValidName(std::string Name);

  /// Convert string input to RTL type
  CGRTLType StrToCGRTL(std::string I);

  /// Convert RTL Type to String
  std::string CGRTLToStr(CGRTLType T);

  /// Retrieve the line number of the target node
  unsigned GetLineNum(const YAML::Node Node);

  /// Automates the recording of parser errors
  void PrintParserError(const YAML::Node Node,
                        std::string Name,
                        std::string Token);

  /// Checks the target Yaml node for valid state
  bool CheckValidNode(const YAML::Node Node,
                      std::string Token );

  /// Checks the target Yaml node for valid state that is a map
  bool CheckValidMapNode(const YAML::Node Node,
                      std::string Token );

  /// Checks the target Yaml node for valid state that is a sequence
  bool CheckValidSequenceNode(const YAML::Node Node,
                              std::string Token );

  /// Yaml Printer for Caches
  void PrintCache( YAML::Emitter *out, CoreGenCache *Cache,
                   std::vector<std::string> &Printed );

  /// Write Yaml: SoC's
  void WriteSocYaml(YAML::Emitter *out, std::vector<CoreGenSoC *> const &Socs );

  /// Write Yaml: Cores
  void WriteCoreYaml(YAML::Emitter *out, std::vector<CoreGenCore *> const &Cores );

  /// Write Yaml: Caches
  void WriteCacheYaml(YAML::Emitter *out, std::vector<CoreGenCache *> const &Caches );

  /// Write Yaml: ISAs
  void WriteISAYaml(YAML::Emitter *out, std::vector<CoreGenISA *> const &ISAs);

  /// Write Yaml: Inst Formats
  void WriteInstFormatYaml(YAML::Emitter *out, std::vector<CoreGenInstFormat *> const &Formats);

  /// Write Yaml: Insts
  void WriteInstYaml(YAML::Emitter *out, std::vector<CoreGenInst *> const &Insts);

  /// Write Yaml: Pseudo Insts
  void WritePseudoInstYaml(YAML::Emitter *out, std::vector<CoreGenPseudoInst *> const &PInsts);

  /// Write Yaml: Register Classes
  void WriteRegClassYaml(YAML::Emitter *out, std::vector<CoreGenRegClass *> const &RegClasses);

  /// Write Yaml: Registers
  void WriteRegYaml(YAML::Emitter *out, std::vector<CoreGenReg *> const &Regs);

  /// Write Yaml: Comms
  void WriteCommYaml(YAML::Emitter *out, std::vector<CoreGenComm *> const &Comms);

  /// Write Yaml: Spads
  void WriteSpadYaml( YAML::Emitter *out, std::vector<CoreGenSpad *> const &Spads);

  /// Write Yaml: Memory Controllers
  void WriteMCtrlYaml( YAML::Emitter *out, std::vector<CoreGenMCtrl *> const &MCtrls);

  /// Write Yaml: Virtual to Physical Units
  void WriteVTPYaml( YAML::Emitter *out, std::vector<CoreGenVTP *> const &VTPs);

  /// Write Yaml: Exts
  void WriteExtYaml(YAML::Emitter *out, std::vector<CoreGenExt *> const &Exts);

  /// Write Yaml: Plugins
  void WritePluginYaml(YAML::Emitter *out, std::vector<CoreGenPlugin *> const &Plugins);

  /// Write Yaml: Project
  void WriteProjYaml(YAML::Emitter *out, CoreGenProj *Proj);

  ///Remove all instances of . from a String
  std::string PrepForASP(std::string RemStr);

  /// Read Yaml: Registers
  bool ReadRegisterYaml(const YAML::Node& RegNodes,
                        std::vector<CoreGenReg *> &Regs);

  /// Read Yaml: Register classes
  bool ReadRegisterClassYaml(const YAML::Node& RegClassNodes,
                             std::vector<CoreGenRegClass *> &RegClasses,
                             std::vector<CoreGenReg *> &Regs);

  /// Read Yaml: ISAs
  bool ReadISAYaml(const YAML::Node& ISANodes,
                   std::vector<CoreGenISA *> &ISAs);

  /// Read Yaml: Instruction Formats
  bool ReadInstFormatYaml(const YAML::Node& InstFormatNodes,
                          std::vector<CoreGenInstFormat *> &Formats,
                          std::vector<CoreGenISA *> &ISAs,
                          std::vector<CoreGenRegClass *> &RegClasses);

  /// Read Yaml: Instructions
  bool ReadInstYaml(const YAML::Node& InstNodes,
                    std::vector<CoreGenInst *> &Insts,
                    std::vector<CoreGenInstFormat *> &Formats,
                    std::vector<CoreGenISA *> &ISAs);

  /// Read Yaml: Pseudo Instructions
  bool ReadPseudoInstYaml(const YAML::Node& PInstNodes,
                          std::vector<CoreGenPseudoInst *> &PInsts,
                          std::vector<CoreGenInst *> &Insts,
                          std::vector<CoreGenISA *> &ISA);

  /// Read Yaml: Caches
  bool ReadCacheYaml(const YAML::Node& CacheNodes,
                     std::vector<CoreGenCache *> &Caches);

  /// Read Yaml: Cores
  bool ReadCoreYaml(const YAML::Node& CoreNodes,
                    std::vector<CoreGenCore *> &Cores,
                    std::vector<CoreGenCache *> &Caches,
                    std::vector<CoreGenISA *> &ISAs,
                    std::vector<CoreGenRegClass *> &RegClasses,
                    std::vector<CoreGenExt *> &Exts);

  /// Read Yaml: Socs
  bool ReadSocYaml(const YAML::Node& SocNodes,
                   std::vector<CoreGenSoC *> &Socs,
                   std::vector<CoreGenCore *> &Cores );

  /// Read Yaml: Spads
  bool ReadSpadYaml(const YAML::Node& SpadNodes,
                    std::vector<CoreGenSpad *> &Spads);

  /// Read Yaml: Memory Controllers
  bool ReadMCtrlYaml(const YAML::Node& MCtrlNodes,
                     std::vector<CoreGenMCtrl *> &MCtrls);

  /// Read Yaml: Virtual to Physical Units
  bool ReadVTPYaml(const YAML::Node& VTPNodes,
                   std::vector<CoreGenVTP *> &VTPs);

  /// Read Yaml: Exts
  bool ReadExtYaml(const YAML::Node& ExtNodes,
                   std::vector<CoreGenExt *> &Exts);

  /// Read Yaml: Plugins
  bool ReadPluginYaml(const YAML::Node& PluginNodes,
                      std::vector<CoreGenPlugin *> &Plugins);

  /// Read Yaml: Project
  bool ReadProjYaml(const YAML::Node& ProjNodes);

  /// Read Yaml: Comms
  bool ReadCommYaml( const YAML::Node& CommNodes,
                     std::vector<CoreGenSoC *>  &Socs,
                     std::vector<CoreGenCore *>  &Cores,
                     std::vector<CoreGenCache *>  &Caches,
                     std::vector<CoreGenISA *>  &ISAs,
                     std::vector<CoreGenInstFormat *>  &Formats,
                     std::vector<CoreGenInst *>  &Insts,
                     std::vector<CoreGenPseudoInst *>  &PInsts,
                     std::vector<CoreGenRegClass *>  &RegClasses,
                     std::vector<CoreGenReg *>  &Regs,
                     std::vector<CoreGenComm *> &Comms,
                     std::vector<CoreGenSpad *> &Spads,
                     std::vector<CoreGenMCtrl *> &MCtrls,
                     std::vector<CoreGenVTP *> &VTPs,
                     std::vector<CoreGenExt *>  &Exts,
                     std::vector<CoreGenPlugin *> &Plugins );

  /// Convert a node name to the target CoreGenNode pointer
  CoreGenNode *NameToNode( std::string Target,
                           std::vector<CoreGenSoC *>  &Socs,
                           std::vector<CoreGenCore *>  &Cores,
                           std::vector<CoreGenCache *>  &Caches,
                           std::vector<CoreGenISA *>  &ISAs,
                           std::vector<CoreGenInstFormat *>  &Formats,
                           std::vector<CoreGenInst *>  &Insts,
                           std::vector<CoreGenPseudoInst *>  &PInsts,
                           std::vector<CoreGenRegClass *>  &RegClasses,
                           std::vector<CoreGenReg *>  &Regs,
                           std::vector<CoreGenComm *> &Comms,
                           std::vector<CoreGenSpad *> &Spads,
                           std::vector<CoreGenMCtrl *> &MCtrls,
                           std::vector<CoreGenVTP *> &VTPs,
                           std::vector<CoreGenExt *>  &Exts,
                           std::vector<CoreGenPlugin *> &Plugins );

  /// Determine if the target node is a duplicate
  bool IsDuplicate( const YAML::Node Node,
                    CoreGenNode *N,
                    std::vector<CoreGenNode *> V);

  /// Checks the target name against the known set of plugin nodes
  CoreGenNode *CheckOverridePlugin(std::string Name);

public:
  /// Default Constructor
  CoreGenYaml(std::string FName, CoreGenPluginMgr *Mgr,
              CoreGenEnv *En, CoreGenProj *Pr, CoreGenErrno *E);

  /// Default Destructor
  ~CoreGenYaml();

  /// Write the internal state to the output Yaml
  bool WriteYaml( std::vector<CoreGenSoC *> const &Socs,
                  std::vector<CoreGenCore *> const &Cores,
                  std::vector<CoreGenCache *> const &Caches,
                  std::vector<CoreGenISA *> const &ISAs,
                  std::vector<CoreGenInstFormat *> const &Formats,
                  std::vector<CoreGenInst *> const &Insts,
                  std::vector<CoreGenPseudoInst *> const &PInsts,
                  std::vector<CoreGenRegClass *> const &RegClasses,
                  std::vector<CoreGenReg *> const &Regs,
                  std::vector<CoreGenComm *> const &Comms,
                  std::vector<CoreGenSpad *> const &Spads,
                  std::vector<CoreGenMCtrl *> const &MCtrls,
                  std::vector<CoreGenVTP *> const &VTPs,
                  std::vector<CoreGenExt *> const &Exts,
                  std::vector<CoreGenPlugin *> const &Plugins );

  /// Read the Yaml into CoreGen structures
  bool ReadYaml( std::vector<CoreGenSoC *>  &Socs,
                 std::vector<CoreGenCore *>  &Cores,
                 std::vector<CoreGenCache *>  &Caches,
                 std::vector<CoreGenISA *>  &ISAs,
                 std::vector<CoreGenInstFormat *>  &Formats,
                 std::vector<CoreGenInst *>  &Insts,
                 std::vector<CoreGenPseudoInst *>  &PInsts,
                 std::vector<CoreGenRegClass *>  &RegClasses,
                 std::vector<CoreGenReg *>  &Regs,
                 std::vector<CoreGenComm *> &Comms,
                 std::vector<CoreGenSpad *> &Spads,
                 std::vector<CoreGenMCtrl *> &MCtrls,
                 std::vector<CoreGenVTP *> &VTPs,
                 std::vector<CoreGenExt *>  &Exts,
                 std::vector<CoreGenPlugin *> &Plugins );

};

#endif

// EOF
