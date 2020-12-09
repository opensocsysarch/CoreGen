//
// _PoarIO_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarIO
 *
 * \ingroup Poar
 *
 * \brief Power/Area speculation IO class
 *
 */

#ifndef _POARIO_H_
#define _POARIO_H_

//-- C++ headers
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

//-- Yaml headers
#include "yaml-cpp/yaml.h"

//-- CoreGen
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

//-- Poar headers
#include "CoreGen/Poar/PoarConfig.h"

class PoarIO{
public:
  /// PoarIO: default constructor
  PoarIO(PoarConfig *Config);

  /// PoarIO: default destructor
  ~PoarIO();

  /// PoarIO: Write textual output
  bool WriteText();

  /// PoarIO: Write Yaml output
  bool WriteYaml();

  /// PoarIO: Write Latex output
  bool WriteLatex();

  /// PoarIO: Write XML output
  bool WriteXML();

private:
  PoarConfig *PConfig; ///< Poar configuration value

  /// PoarIO: Get the total power
  double GetTotalPower();

  /// PoarIO: Get the total area
  double GetTotalArea();
};

#endif

// EOF
