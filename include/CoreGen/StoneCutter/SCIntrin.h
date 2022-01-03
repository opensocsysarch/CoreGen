//
// _SCIntrin_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCIntrin
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter intrinsic base class
 *
 */

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

// CoreGen headers
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

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
  SCIntrin( unsigned NI, std::string K, bool F )
    : NumInputs(NI), Keyword(K), isFOp(F) {}

  /// Standard destructor
  ~SCIntrin() {};

  /// Retrieve the number of required inputs
  unsigned GetNumInputs() { return NumInputs; }

  /// Retrieve the target keyword
  std::string GetKeyword() { return Keyword; }

  /// Retrieve the error message
  std::string GetErrMsg() { return ErrMsg; }

  /// Determine whether the target intrinsic implements a fused operation
  bool IsFusedOp() { return isFOp; }

  /// Determine width of first intrinsic argument (TODO: Make for all operands) 
  unsigned DeriveWidth(Instruction &I){
    unsigned width = 0;
    if( auto CInt = dyn_cast<ConstantInt>(I.getOperand(0)) ){
      width = (unsigned)(CInt->getZExtValue());
    }else{
      width = I.getOperand(0)->getType()->getIntegerBitWidth();
    }
    return width;
  }
  // virtual functions

  /// Executes the intrinsic codegenerator
  virtual Value *codegen() { return nullptr; }

  /// Generates the intrinsic signal map
  virtual bool GetSigMap(CoreGenSigMap *Sigs,Instruction &I,std::string Inst){
    for( auto i : ISignals ){
      std::string Str;
      if( MDNode *N = I.getMetadata("pipe.pipeName")) {
        Str = cast<MDString>(N->getOperand(0))->getString().str();
      }
      Sigs->InsertSignal(new SCSig(i->GetType(), i->GetWidth(), Inst, Str));
    }
    return true;
  }

protected:
  std::vector<SCSig *> ISignals;    ///< Intrinsic Signals
  std::string ErrMsg;               ///< Error Message

private:
  unsigned NumInputs;               ///< Number of required input arguments
  std::string Keyword;              ///< Intrinsic Keyword
  bool isFOp;                       ///< Is the intrinsic a fused operation?
};

#endif

// EOF
