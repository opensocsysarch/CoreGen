//
// _SCSigMap_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCSigMap
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Generates a signal map for the target ISA
 *
 */

#ifndef _STONECUTTER_SCPASS_SCSIGMAP_H_
#define _STONECUTTER_SCPASS_SCSIGMAP_H_

// standard headers
#include <map>
#include <vector>
#include <fstream>

// YAML headers
#include "yaml-cpp/yaml.h"

// CoreGen headers
#include "CoreGen/StoneCutter/SCPass.h"
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"


class SCSigMap : public SCPass {
private:

  // Private variables
  CoreGenSigMap *Signals;           ///< CoreGenSigMap handler
  std::string SigMap;               ///< Signal map output file
  std::vector<SCIntrin *>* Intrins; ///< Intrinsics vector

  // Private functions
  /// walks the LLVM Module object and discovers all the interior signals
  bool DiscoverSigMap();

  /// Decodes an individual instruction for the signal map
  bool CheckSigReq( Function &F, Instruction &I );

  /// Decodes the PC signals for each instruction within a function
  bool CheckPCReq( Function &F );

  /// Translates binary operations to signals
  bool TranslateBinaryOp( Function &F, Instruction &I, SigType Type );

  /// Translates logical operations to signals
  bool TranslateLogicalOp( Function &F, Instruction &I, SigType Type );

  /// Translates memory operations to signals
  bool TranslateMemOp( Function &F, Instruction &I );

  /// Translates select operations to mux signals
  bool TranslateSelectSig( Function &F, Instruction &I );

  /// Translates the I/O operands to signals
  bool TranslateOperands( Function &F, Instruction &I );

  /// Translates the target input operands for the ALU instruction
  bool TranslateALUOperands( Function &F, Instruction &I, SCSig *Sig );

  /// Translates the target operand (instruction name)  to signals
  bool TranslateTargetOperands( Function &F, Instruction &I );

  /// Translates the instruction I/O operans to PC enable signals
  bool TranslatePCSig( Instruction &I, bool &PCJump );

  /// Translates the target call operation to the appropriate signals
  bool TranslateCallSig(Function &F, Instruction &I);

  /// Translates the compare operation to the appropriate signals
  bool TranslateCmpOp(Function &F, Instruction &I);

public:
  /// Default cosntructor
  SCSigMap(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCSigMap();

  /// Execute the codegen
  virtual bool Execute() override;

  /// Sets the signal map file name
  bool SetSignalMapFile(std::string SM);

  /// Sets the intrinsics vector
  bool SetIntrins(std::vector<SCIntrin *>* Intrins);
};

#endif

// EOF
