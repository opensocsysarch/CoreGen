//
// _SCVLIWPipeBuilder_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCVLIWPipeBuilder
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Constructs the arithmetic portions of the Chisel pipeline
 *
 */

#ifndef _STONECUTTER_SCPASS_SCVLIWPIPEBUILDER_H_
#define _STONECUTTER_SCPASS_SCVLIWPIPEBUILDER_H_

#include <map>
#include <vector>
#include <tuple>
#include "CoreGen/StoneCutter/SCPass.h"
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

class SCVLIWPipeBuilder : public SCPass {
private:
  CoreGenSigMap *SigMap;              ///< SCVLIWPipeBuilder: Signal map object

  std::vector<std::pair<std::string,bool (SCVLIWPipeBuilder::*)()>> Enabled;   ///< SCVLIWPipeBuilder: Enabled sub-passes

  std::vector<std::pair<std::string,unsigned>> VLIWStages;  ///< SCVLIWPipeBuilder: VLIW Stages

  /// SCVLIWPipeBuilder: Enable sub-passes
  bool EnableSubPasses();

  /// SCVLIWPipeBuilder: Execute the various optimization phases
  bool Optimize();

  /// SCVLIWPipeBuilder: Write the signal map back
  bool WriteSigMap();

  /// SCVLIWPipeBuilder: Derive the pipeline stages that are VLIW
  bool DeriveVLIWStages();

  // ----------------------------------------------------------
  // Sub Passes
  // ----------------------------------------------------------

  /// SCVLIWPipeBuilder SubPass: Determines whether the pipeline stages are monotonically increasing
  bool DiscoverPipeSlots();

public:
  /// Default cosntructor
  SCVLIWPipeBuilder(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCVLIWPipeBuilder();

  /// Set the signal map object
  bool SetSignalMap(CoreGenSigMap *CSM){ SigMap = CSM; return true; }

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif


