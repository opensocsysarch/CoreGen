//
// _SCChiselCodeGen_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCChiselCodeGen
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Chisel code generation class
 *
 */

#ifndef _STONECUTTER_SCCHISELCODEGEN_H_
#define _STONECUTTER_SCCHISELCODEGEN_H_

// standard headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

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
#include "CoreGen/StoneCutter/SCUtil.h"

using namespace llvm;

class SCChiselCodeGen{
private:
  SCParser *Parser;                           ///< SC Parser object
  SCMsg *Msgs;                                ///< SC Messages object
  std::string ChiselFile;                     ///< Output file

  std::ofstream OutFile;                      ///< Output file stream

  // private functions
  void WriteChiselHeader();                   ///< Writes a header to the chisel output file
  bool ExecuteCodegen();                      ///< Generates chisel from the LLVM IR

public:

  /// Default constructor
  SCChiselCodeGen(SCParser *, SCMsg *, std::string);

  /// Default desctrutor
  ~SCChiselCodeGen();

  /// Generate the LLVM Chisel output
  bool GenerateChisel();

};

#endif

// EOF
