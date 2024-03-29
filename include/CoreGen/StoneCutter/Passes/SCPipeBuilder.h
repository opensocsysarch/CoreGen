//
// _SCPipeBuilder_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCPipeBuilder
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Constructs the arithmetic portions of the Chisel pipeline
 *
 */

#ifndef _STONECUTTER_SCPASS_SCPIPEBUILDER_H_
#define _STONECUTTER_SCPASS_SCPIPEBUILDER_H_

#include <map>
#include <vector>
#include <tuple>
#include "CoreGen/StoneCutter/SCPass.h"
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

class SCPipeBuilder : public SCPass {
private:

  CoreGenSigMap *SigMap;              ///< SCPipeBuilder: Signal map object

  std::vector<std::string> PipeVect;  ///< SCPipeBuilder: Pipe vector
  std::vector<std::string> InstVect;  ///< SCPipeBuilder: Instruction vector

  std::vector<std::pair<std::string,bool (SCPipeBuilder::*)()>> Enabled;   ///< SCPipeBuilder: Enabled sub-passes

  std::vector<std::pair<std::string, std::string>> AttrMap;     ///< SCPipeBuilder: Attribute to pipeline pairs

  std::vector<std::pair<std::string,std::string>> ExtStage;     ///< SCPipeBuilder: Tracks the new pipeline to stage relationship

  std::map<std::string,unsigned> NStages;   ///< SCPipeBuilder: Tracks the user-defined pipeline stage counts

  unsigned **AdjMat;                  ///< SCPipeBuilder: Adjacency Matrix

  /// SCPipeBuilder: Initialize the atreibute vector
  bool InitAttrs();

  /// SCPipeBuilder: Build the matrix representation
  bool BuildMat();

  /// SCPipeBuilder: Writes the pipeline back to the signal map
  bool WriteSigMap();

  /// SCPipeBuilder: Allocates the adjacency matrix
  bool AllocMat();

  /// SCPipeBuilder: Frees the adjacency matrix
  bool FreeMat();

  /// SCPipeBuilder: Prints the adjancency matrix for debugging
  void PrintAdjMat();

  /// SCPipeBuilder: Execute the various optimization phases
  bool Optimize();

  /// SCPipeBuilder: Retrives the x-axis idx from the pipe stage name
  unsigned PipeToIdx(std::string P);

  /// SCPipeBuilder: Retrieves the y-axis idx from the signal name
  unsigned SigToIdx(SCSig *S);

  /// SCPipeBuilder: Clears all the enabled pipe stages for the target signal name
  bool ClearSignal(SCSig *S);

  /// SCPipeBuilder: Enable sub-passes
  bool EnableSubPasses();

  /// SCPipeBuilder: Enable performance optimization sub-passes
  bool EnablePerfSubPasses();

  /// SCPipeBuilder: Enable area optimization sub-passes
  bool EnableAreaSubPasses();

  /// SCPipeBuilder: Enable power optimization sub-passes
  bool EnablePowerSubPasses();

  /// SCPipeBuilder: Determines if a pass is enabled
  bool IsSubPassEnabled(std::string Pass);

  /// SCPipeBuilder: Determines if the two signals are "adjacent"
  bool IsAdjacent(SCSig *Base, SCSig *New);

  /// SCPipeBuilder: Does the target pipe have any I/O signals?
  bool HasIOSigs(std::string Pipe);

  /// SCPipeBuilder: Retrieves a vector of empty pipeline stages
  std::vector<std::string> GetEmptyStages();

  /// SCPipeBuilder: Derives the source pipeline from the pipeline stage
  std::string DerivePipeline(std::string Stage);

  // ----------------------------------------------------------
  // Sub Passes
  // ----------------------------------------------------------

  /// SCPipeBuilder SubPass: Splits the pipeline to N-stages if prescribed by the user
  bool SplitNStage();

  /// SCPipeBuilder SubPass: Split the register  and memory IO
  bool SplitIO();

  /// SCPipeBuilder SubPass: Fit the remaining arithmetic ops into adjacement pipe stages
  bool FitArith();

  /// SCPipeBuilder SubPass: Fit the temporary register read/write signals
  bool FitTmpReg();

  /// SCPipeBuilder SubPass: Fit memory signals into appropriate pipe stages
  bool FitMem();

  /// SCPipeBuilder SubPass: Fit the PC update signals
  bool FitPCSigs();

  /// SCPipeBuilder SubPass: Removes dead pipeline stages
  bool DeadPipeElim();

  /// SCPipeBuilder SubPass: Checks for signals with no pipes
  bool EmptySig();

public:
  /// Default cosntructor
  SCPipeBuilder(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCPipeBuilder();

  /// Set the signal map object
  bool SetSignalMap(CoreGenSigMap *CSM){ SigMap = CSM; return true; }

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
