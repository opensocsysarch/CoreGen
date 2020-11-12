//
// _PoarConfig_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarConfig
 *
 * \ingroup Poar
 *
 * \brief Power/Area speculation configuration class
 *
 */

#ifndef _POARCONFIG_H_
#define _POARCONFIG_H_

#include <iostream>
#include <string>
#include <fstream>

#include "yaml-cpp/yaml.h"

class PoarConfig{
public:
  typedef enum{
    // Power Values
    UNK_ENTRY       = 0,        ///< ConfigType: null type
    POWER_REGBIT    = 10,       ///< ConfigType: Power per register bit
    POWER_DPATHBIT  = 11,       ///< ConfigType: Power per data path bit
    POWER_CPATHBIT  = 12,       ///< ConfigType: Power per control path bit
    POWER_CACHEBIT  = 13,       ///< ConfigType: Power per cache bit
    POWER_SPADBIT   = 14,       ///< ConfigType: Power per scratchpad bit
    POWER_ROMBIT    = 15,       ///< ConfigType: Power per ROM bit

    // Area Values
    AREA_REGBIT     = 100,      ///< ConfigType: Area per register bit
    AREA_DPATHBIT   = 101,      ///< ConfigType: Area per data path bit
    AREA_CPATHBIT   = 102,      ///< ConfigType: Area per control path bit
    AREA_CACHEBIT   = 103,      ///< ConfigType: Area per cache bit
    AREA_SPADBIT    = 104,      ///< ConfigType: Area per scratchpad bit
    AREA_ROMBIT     = 105       ///< ConfigType: Area per ROM bi
  }ConfigType;                  ///< PoarConfig: Defines the configuration types

  typedef struct{
    ConfigType Type;    ///< ConfigEntry: configuration type
    std::string Name;   ///< ConfigEntry: name of the config entry
    std::string Accum;  ///< ConfigEntry: name of the corresponding accumulator
    double DefaultVal;  ///< ConfigEntry: default value
    double Value;       ///< ConfigEntry: the value of the entry
  }ConfigEntry;         ///< PoarConfig: configuration entry structure



private:
  std::string Config;   ///< PoarConfig: configuration file path

  /// PoarConfig: read the configuration values
  void ReadConfig();

  /// PoarConfig: load the default values
  void LoadDefaultValues();

  /// PoarConfig: load a single default value
  bool LoadDefaultValue(ConfigType Type);

  /// PoarConfig: print the configuration data (debugging)
  void PrintConfig();

  static ConfigEntry Entries[]; ///< PoarConfig: configuration entries table

public:
  /// PoarConfig: default constructor
  PoarConfig(std::string Config);

  /// PoarConfig: default destructor
  ~PoarConfig();

  /// PoarConfig: retrieve the target configuration
  PoarConfig::ConfigEntry GetEntry(unsigned Entry);

  /// PoarConfig: retrieve the number of entries
  unsigned GetNumEntry();
};

#endif

// EOF
