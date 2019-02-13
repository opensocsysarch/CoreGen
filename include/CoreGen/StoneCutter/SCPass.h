//
// _SCPass_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCPass
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter code generation base class
 *
 */

#ifndef _STONECUTTER_SCPASS_H_
#define _STONECUTTER_SCPASS_H_

// standard headers
#include <string>
#include <vector>

// stonecutter headers
#include "CoreGen/StoneCutter/SCOpts.h"
#include "CoreGen/StoneCutter/SCMsg.h"

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

using namespace llvm;

class SCPass{
private:
  std::string Name;                   ///< Name of the StoneCutter pass
  SCMsg *Msgs;                        ///< StoneCutter messages output

protected:
  SCOpts *Opts;                       ///< StoneCutter options
  Module *TheModule;                  ///< LLVM IR Module

public:

  /// Default constructor
  SCPass(std::string N,
         Module *TM,
         SCOpts *O,
         SCMsg *M);

  /// Default destructor
  ~SCPass();

  /// Retrieve the name of the pass
  std::string GetName() { return Name; }

  /// Prints a message to the message stream
  void PrintMsg( MSG_LEVEL L, const std::string M );

  /// Prints a raw message to the message stream
  void PrintRawMsg( const std::string M );

  /// Determines if the target variable is a global variable
  bool IsGlobal( std::string Var );

  /// Determines if the target variable has the target attribute
  bool HasGlobalAttribute(std::string Var, std::string Attribute );

  /// Retrieves the target attribute from the target global variable
  std::string GetGlobalAttribute(std::string Var, std::string Attribute);

  /// Executes the target code generation pass
  virtual bool Execute() = 0;

};

#endif

// EOF
