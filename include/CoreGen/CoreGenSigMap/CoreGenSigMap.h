//
// _CoreGenSigMap_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenSigMap
 *
 * \ingroup CoreGenSigMap
 *
 * \brief CoreGen Signal Map Library
 *
 */

#ifndef _COREGENSIGMAP_H_
#define _COREGENSIGMAP_H_

#include <vector>
#include <fstream>
#include <algorithm>

// YAML headers
#include "yaml-cpp/yaml.h"

// CoreGen headers
#include "CoreGen/CoreGenSigMap/SCSig.h"

class CoreGenSigMap{
private:
  // private variables
  std::vector<SCSig *> Signals;

  // private functions
  /// Writes the top-level signal map to the YAML file
  bool WriteTopLevelSignals(YAML::Emitter *out);

  /// Writes the individual instruction signal map to the YAML file
  bool WriteInstSignals(YAML::Emitter *out);

public:

  /// Default constructor
  CoreGenSigMap();

  /// Default destructor
  ~CoreGenSigMap();

  /// Inserts a signal into the signal map
  bool InsertSignal( SCSig *Sig );

  /// Retrieves the target signal from the signal map
  SCSig *GetSignal( unsigned Idx );

  /// Retrieves the number of signals in the signal map
  unsigned GetNumSignals() { return Signals.size(); }

  /// Write the signal map to the target file
  bool WriteSigMap( std::string File );

  /// Reads the signal map file into the signal structure
  bool ReadSigMap( std::string File );
};

#endif

// EOF
