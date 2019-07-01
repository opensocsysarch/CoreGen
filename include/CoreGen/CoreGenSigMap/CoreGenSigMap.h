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
#include "CoreGen/CoreGenSigMap/SCTmp.h"

class CoreGenSigMap{
private:
  // private variables
  std::vector<SCSig *> Signals;

  std::vector<SCTmp *> TempRegs;

  std::vector<SigType> TopSigs;

  unsigned TmpIdx;

  // private functions
  /// Writes the top-level signal map to the YAML file
  bool WriteTopLevelSignals(YAML::Emitter *out);

  /// Writes the individual instruction signal map to the YAML file
  bool WriteInstSignals(YAML::Emitter *out);

  /// Writes the temporary register info and the associated mappings to the YAML file
  bool WriteTempRegs(YAML::Emitter *out);

  /// Determines if the target YAML node is valid
  bool CheckValidNode(const YAML::Node Node, std::string Token);

  /// Reads the top-level signal map
  bool ReadTopLevelSignals(const YAML::Node& TopNodes);

  /// Reads the instruction-level signal map
  bool ReadInstSignals(const YAML::Node& InstNodes);

  /// Reads the temporary register value mappings
  bool ReadTmpRegs(const YAML::Node& TmpNodes);

  /// Converts the string to a signal name
  SigType StrToSigType( std::string Sig );

public:

  /// Default constructor
  CoreGenSigMap();

  /// Default destructor
  ~CoreGenSigMap();

  /// Inserts a signal into the signal map
  bool InsertSignal( SCSig *Sig );

  /// Retrieves the target signal from the signal map
  SCSig *GetSignal( unsigned Idx );

  /// Retrieves the maximum signal width for the target signal
  unsigned GetMaxSignalWidth( unsigned Idx );

  /// Retrieves the minimum signal width for the target signal
  unsigned GetMinSignalWidth( unsigned Idx );

  /// Retrieves the number of signals in the signal map
  unsigned GetNumSignals() { return Signals.size(); }

  /// Write the signal map to the target file
  bool WriteSigMap( std::string File );

  /// Reads the signal map file into the signal structure
  bool ReadSigMap( std::string File );

  /// Attempt to retrieve a temporary register for the target instruction:IRName mapping
  std::string GetTempReg( std::string Inst, std::string IRName, unsigned width );

  /// Retrieve the existing mapping for the Inst:IRName pair
  std::string GetTempMap( std::string Inst, std::string IRName );
};

#endif

// EOF
