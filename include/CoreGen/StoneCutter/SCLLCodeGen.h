//
// _SCLLCodeGen_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCLLCodeGen
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter LLVM IR and BC code generation class
 *
 */

#ifndef _STONECUTTER_SCLLCODEGEN_H_
#define _STONECUTTER_SCLLCODEGEN_H_

// standard headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

// llvm headers
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

// coregen headers
#include "CoreGen/StoneCutter/SCParser.h"
#include "CoreGen/StoneCutter/SCMsg.h"

using namespace llvm;

class SCLLCodeGen{
private:
  SCParser *Parser;                           ///< SC Parser object
  SCMsg *Msgs;                                ///< SC Messages object
  std::string OutputFile;                     ///< Output file
  std::string ObjFile;                        ///< Object file

  // private functions

public:

  /// Default constructor
  SCLLCodeGen(SCParser *, SCMsg *, std::string, std::string);

  /// Default desctrutor
  ~SCLLCodeGen();

  /// Generate the LLVM IR output
  bool GenerateLL();

  /// Generate the LLVM object file
  bool GenerateObjFile();
};

#endif

// EOF
