//
// _CoreGenSigMap_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
#include <string>

// YAML headers
#include "yaml-cpp/yaml.h"

// CoreGen headers
#include "CoreGen/CoreGenSigMap/SCSig.h"
#include "CoreGen/CoreGenSigMap/SCTmp.h"
#include "CoreGen/CoreGenSigMap/SMPasses.h"

class CoreGenSigMap{
private:
  // private variables
  std::vector<SCSig *> Signals;   ///< CoreGenSigMap: signal vector

  std::vector<SCTmp *> TempRegs;  ///< CoreGenSigMap: temporary register vector

  std::vector<SigType> TopSigs;   ///< CoreGenSigMap: top-level signals

  std::vector<std::string> Pipelines; ///< CoreGenSigMap: pipelines
  std::vector<std::pair<std::string,std::string>> PipeAttrs;  ///< CoreGenSigMap: pipeline attributes <pipeline:attr>
  std::vector<std::pair<std::string,std::string>> PipeStages; ///< CoreGenSigMap: pipeline stages <pipeline:stage>

  unsigned TmpIdx;                ///< CoreGenSigMap: Temporary index

  std::string Error;              ///< CoreGenSigMap: Error string

  std::string SigFile;            ///< CoreGenSigMap: Output file

  // private functions
  /// Writes the top-level signal map to the YAML file
  bool WriteTopLevelSignals(YAML::Emitter *out);

  /// Writes the individual instruction signal map to the YAML file
  bool WriteInstSignals(YAML::Emitter *out);

  /// Writes the temporary register info and the associated mappings to the YAML file
  bool WriteTempRegs(YAML::Emitter *out);

  /// Writes the pipeline information to the YAML file
  bool WritePipeData(YAML::Emitter *out);

  /// Determines if the target YAML node is valid
  bool CheckValidNode(const YAML::Node Node, std::string Token);

  /// Reads the top-level signal map
  bool ReadTopLevelSignals(const YAML::Node& TopNodes);

  /// Reads the instruction-level signal map
  bool ReadInstSignals(const YAML::Node& InstNodes);

  /// Reads the pipeline structure from the signal map
  bool ReadPipelineData(const YAML::Node& PipeNodes);

  /// Reads the temporary register value mappings
  bool ReadTmpRegs(const YAML::Node& TmpNodes);

  /// Converts the string to a signal name
  SigType StrToSigType( std::string Sig );

  /// Converst the string to a fused op name
  FusedOpType StrToFusedOpType( std::string Fop );

public:

  /// Default constructor
  CoreGenSigMap();

  /// Default destructor
  ~CoreGenSigMap();

  /// Retrieves the internal error string for any failed operations
  std::string GetErrStr() { return Error; }

  /// Retrieves the number of unique instructions represented in the signal map
  unsigned GetNumInst();

  /// Retrieves a vector of all the instruction names in the signal map
  std::vector<std::string> GetInstVect();

  /// Retrieves a vector of all the signals for the target instruction
  std::vector<SCSig *> GetSigVect(std::string Inst);

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

  /// Retrieves a vector of the pipeline stages found in the included signals
  std::vector<std::string> GetPipeVect();

  /// Write the signal map
  bool WriteSigMap();

  /// Write the signal map to the target file
  bool WriteSigMap( std::string File );

  /// Reads the signal map file into the signal structure
  bool ReadSigMap( std::string File );

  /// Retrieve the number of temporary registers that need to exist in the ALUL
  unsigned GetNumTemps() { return TempRegs.size(); }

  /// Attempt to retrieve a temporary register for the target instruction:IRName mapping
  std::string GetTempReg( std::string Inst, std::string IRName, unsigned width );

  /// Retrieve the existing mapping for the Inst:IRName pair
  std::string GetTempMap( std::string Inst, std::string IRName );

  /// Insert a pipeline
  bool InsertPipeline(std::string Pipeline);

  /// Retrieve the pipeline vector
  std::vector<std::string> GetPipelines() { return Pipelines; }

  /// Insert a pipeline stage into the target pipeline
  bool InsertPipelineStage(std::string Pipeline, std::string Stage);

  /// Retrieve the number of pipeline stages for the target pipeline
  unsigned GetNumPipeStages(std::string Pipeline);

  /// Retrieve the target stage for the target pipeline
  std::string GetPipelineStage( std::string Pipeline, unsigned Stage );

  /// Insert a pipeline attribute into the target pipeline
  bool InsertPipelineAttr(std::string Pipeline, std::string Attr);

  /// Retrieve the number of pipeline attributes for the target pipeline
  unsigned GetNumPipeAttrs(std::string Pipeline);

  /// Retrieve the target attribute for the target pipeline stage
  std::string GetPipelineAttr(std::string Pipeline, unsigned Stage );

  /// Execute all the signal map passes
  bool ExecutePasses();
};

#endif

// EOF
