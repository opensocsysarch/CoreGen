//
// _SCMetadataAdjust_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCIMetadataAdjust
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Adjusts pipeline metadata blocks that have
 *                                  been modified by the standard set of LLVM
 *                                  passes.
 *
 */

#ifndef _STONECUTTER_SCPASS_SCMETADATAADJUST_H_
#define _STONECUTTER_SCPASS_SCMETADATAADJUST_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCMetadataAdjust : public SCPass {
private:

  /// Adjusts the metadata that has been dropped due to LLVM optimizations
  void AdjustMD();

  /// Examines the target basic block for metadata continuity
  void ExamineBBMetadata(BasicBlock &BB);

  /// Sets the metadata for the target instruction
  void SetMetadata(BasicBlock &BB,
                   unsigned Instruction,
                   MDNode *NameNode,
                   MDNode *LineNode);

public:
  /// Default cosntructor
  SCMetadataAdjust(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCMetadataAdjust();

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif

// EOF
