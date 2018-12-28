//
// _SCIntrin_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCINTRIN_H_
#define _STONECUTTER_SCINTRIN_H_

// LLVM headers
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

// Standard headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

class SCIntrin{
public:
  /// Standard constructor
  SCIntrin();

  /// Overloaded Constructor
  SCIntrin( unsigned NI, std::string K ) : NumInputs(NI), Keyword(K) {}

  /// Standard destructor
  ~SCIntrin();

  /// Retrieve the number of required inputs
  unsigned GetNumInputs() { return NumInputs; }

  /// Retrieve the target keyword
  std::string GetKeyword() { return Keyword; }

  // virtual functions

  /// Executes the intrinsic codegenerator
  virtual Value *codegen() { return nullptr; }

private:
  unsigned NumInputs;               ///< Number of required input arguments
  std::string Keyword;              ///< Intrinsic Keyword
};

#endif

// EOF