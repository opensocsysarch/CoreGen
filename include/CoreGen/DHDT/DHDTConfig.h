//
// _DHDTConfig_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class DHDTConfig
 *
 * \ingroup DHDT
 *
 * \brief DHDT configuration class
 *
 */

#ifndef _DHDTCONFIG_H_
#define _DHDTCONFIG_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "yaml-cpp/yaml.h"

class DHDTConfig{
public:

  typedef enum{
    // generic components
    UNK_ENTRY   = 0,      ///< ConfigType: null type
    REGBIT      = 10,     ///< ConfigType: Power per register bit
    DPATHBIT    = 11,     ///< ConfigType: Power per data path bit
    CPATHBIT    = 12,     ///< ConfigType: Power per control path bit
    SPADBIT     = 13,     ///< ConfigType: Power per scratchpad bit
    ROMBIT      = 14,     ///< ConfigType: Power per ROM bit

    // ALU control/data components
    ALUREGBIT   = 50,     ///< ConfigType: Power per ALU register bit
    ALUDPATH    = 51,     ///< ConfigType: Power per ALU data path bit
    ALUCPATH    = 52,     ///< ConfigType: Power per ALU control path bit

    // ALU Ops
    ALUADD      = 60,     ///< ConfigType: Power per ALU Add bit
    ALUFADD     = 61,     ///< ConfigType: Power per ALU FAdd bit
    ALUSUB      = 62,     ///< ConfigType: Power per ALU Sub bit
    ALUFSUB     = 63,     ///< ConfigType: Power per ALU FSub bit
    ALUMUL      = 64,     ///< ConfigType: Power per ALU Mul bit
    ALUFMUL     = 65,     ///< ConfigType: Power per ALU FMul bit
    ALUUDIV     = 66,     ///< ConfigType: Power per ALU UDiv bit
    ALUSDIV     = 67,     ///< ConfigType: Power per ALU SDiv bit
    ALUFDIV     = 68,     ///< ConfigType: Power per ALU FDiv bit
    ALUUREM     = 69,     ///< ConfigType: Power per ALU URem bit
    ALUSREM     = 70,     ///< ConfigType: Power per ALU SRem bit
    ALUFREM     = 71,     ///< ConfigType: Power per ALU FRem bit
    ALUSHL      = 72,     ///< ConfigType: Power per ALU Shl bit
    ALULSHR     = 73,     ///< ConfigType: Power per ALU LShr bit
    ALUASHR     = 74,     ///< ConfigType: Power per ALU AShr bit
    ALUAND      = 75,     ///< ConfigType: Power per ALU And bit
    ALUOR       = 76,     ///< ConfigType: Power per ALU Or bit
    ALUXOR      = 77,     ///< ConfigType: Power per ALU Xor bit
    ALUICMP     = 78,     ///< ConfigType: Power per ALU ICmp bit
    ALUFCMP     = 79,     ///< ConfigType: Power per ALU FCmp bit
    ALUMUX      = 80,     ///< ConfigType: Power per ALU Mux bit

    // ALU Intrinsic Ops
    ALUMAX      = 81,     ///< ConfigType: Power per ALU Max bit
    ALUMIN      = 82,     ///< ConfigType: Power per ALU Min bit
    ALUNOT      = 83,     ///< ConfigType: Power per ALU Not bit
    ALUNEG      = 84,     ///< ConfigType: Power per ALU Neg bit
    ALUREV      = 85,     ///< ConfigType: Power per ALU Reverse bit
    ALUPOPC     = 86,     ///< ConfigType: Power per ALU PopCount bit
    ALUCLZ      = 87,     ///< ConfigType: Power per ALU CountLZ bit
    ALUCTZ      = 88,     ///< ConfigType: Power per ALU CountTZ bit
    ALUSEXT     = 89,     ///< ConfigType: Power per ALU SEXT bit
    ALUZEXT     = 90,     ///< ConfigType: Power per ALU ZEXT bit
    ALUROTL     = 91,     ///< ConfigType: Power per ALU RotL bit
    ALUROTR     = 92,     ///< ConfigType: Power per ALU RotR bit
    ALUMAJ      = 93,     ///< ConfigType: Power per ALU Majority vote bit
    ALUDOZ      = 94,     ///< ConfigType: Power per ALU Doz bit
    ALUCOMP     = 95,     ///< ConfigType: Power per ALU Compress bit
    ALUCOMPM    = 96,     ///< ConfigType: Power per ALU CompressM bit
    ALUINSERTS  = 97,     ///< ConfigType: Power per ALU InsertS bit
    ALUINSERTZ  = 98,     ///< ConfigType: Power per ALU InsertZ bit
    ALUEXTRS    = 99,     ///< ConfigType: Power per ALU ExtractS bit
    ALUEXTRZ    = 100,    ///< ConfigType: Power per ALU ExtractZ bit
    ALUMERGE    = 101,    ///< ConfigType: Power per ALU Merge bit
    ALUCONCAT   = 102,    ///< ConfigType: Power per ALU Concat bit
    ALULSS      = 103,    ///< ConfigType: Power per ALU LSS bit
    ALUFENCE    = 104,    ///< ConfigType: Power per ALU Fence bit
    ALUBSEL     = 105,    ///< ConfigType: Power per ALU BSel bit
    ALUNOP      = 106,    ///< ConfigType: Power per NOP

    // Register Ops
    REGLOAD     = 200,    ///< ConfigType: Power per register bit load
    REGSTORE    = 201,    ///< ConfigType: Power per register bit store

    // Memory Ops
    MEMLOAD     = 300,    ///< ConfigType: Power per memory load
    MEMSTORE    = 301,    ///< ConfigType: Power per memory store

  }ConfigType;            ///< DHDTConfig: Defines the configuration types

  typedef struct{
    ConfigType Type;      ///< ConfigEntry: configuration type
    std::string Name;     ///< ConfigEntry: name of the config entry
    double DefaultVal;    ///< ConfigEntry: default value
    double Value;         ///< ConfigEntry: value
  }ConfigEntry;           ///< DHDTConfig: configuration entry structure

private:
  std::string Config;     ///< DHDTConfig: configuration file path

  /// DHDTConfig: read the configuration values
  void ReadConfig();

  /// DHDTConfig: load the default values
  void LoadDefaultValues();

  /// DHDTConfig: load a single default value
  bool LoadDefaultValue(ConfigType Type);

  /// DHDTConfig: print the configuration data
  void PrintConfig();

  static ConfigEntry Entries[];     ///< DHDTConfig: configuration entries table

public:
  /// DHDTConfig: default constructor
  DHDTConfig(std::string Config);

  /// DHDTConfig: default destructor
  ~DHDTConfig();

  /// DHDTConfig: retrieve the target cofiguration
  DHDTConfig::ConfigEntry GetEntry(unsigned Entry);

  /// DHDTConfig: retrieve the number of entries
  unsigned GetNumEntry();

};

#endif

// EOF
