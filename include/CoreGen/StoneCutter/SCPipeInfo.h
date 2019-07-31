//
// _SCPipeInfo_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCPipeInfo
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter code generation pipe information class
 *
 */

#ifndef _STONECUTTER_SCPIPEINFO_H_
#define _STONECUTTER_SCPIPEINFO_H_

// standard headers
#include <string>
#include <vector>

// stonecutter headers
#include "CoreGen/StoneCutter/SCOpts.h"
#include "CoreGen/StoneCutter/SCMsg.h"
#include "CoreGen/StoneCutter/SCIntrinsics.h"

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

class SCPipeInfo{
private:
  Module *TheModule;                  ///< LLVM IR Module
  SCOpts *Opts;                       ///< StoneCutter options
  SCMsg *Msgs;                        ///< StoneCutter messages output

  bool PCPresent;                     ///< Determines whether the ISA has an explicit PC register

  std::vector<std::string> UniqueRegFields; ///< Vector of unique register field names
  std::vector<std::string> UniqueImmFields; ///< Vector of unique imm field names
  std::vector<std::pair<std::string,std::string>> RegFields;  ///< Vector of InstFormat:RegField names
  std::vector<std::pair<std::string,std::string>> ImmFields;  ///< Vector of InstFormat:ImmField names
  std::pair<std::string,std::string> PCReg; ///< PC Reg info in the form RegClass:PCRegName

public:
  /// Default constructor
  SCPipeInfo(Module *TM,
             SCOpts *O,
             SCMsg *M);

  /// Default Destructor
  ~SCPipeInfo();

  /// Retrieves the number of register field names
  unsigned GetNumRegFields() { return RegFields.size(); }

  /// Retrieves the instruction format name for the target register index
  std::string GetRegFieldInstFormat( unsigned Idx );

  /// Retrieves the register field name for the target register index
  std::string GetRegFieldName( unsigned Idx );

  /// Retrieves the number of unique register field names
  unsigned GetNumUniqueRegFields() { return UniqueRegFields.size(); }

  /// Retrieves the register name for the target unique register index
  std::string GetUniqueFieldName( unsigned Idx );

  /// Retrieves the number of unique immediate field names
  unsigned GetNumImmFields() { return ImmFields.size(); }

  /// Retrieves the instruction format name for the target immediate index
  std::string GetImmFieldInstFormat( unsigned Idx );

  /// Retrieves the immediate field name for the target immediate index
  std::string GetImmFieldName( unsigned Idx );

  /// Retrieves the number of unique imediate field names
  unsigned GetNumUniqueImmFields() { return UniqueImmFields.size(); }

  /// Retrieves the immediate field name for the target unique immediate index
  std::string GetUniqueImmFieldName( unsigned Idx );

  /// Determines if there is an explicit PC present in the ISA
  bool IsPCPresent() { return PCPresent; }

  /// Retrieves the name of the PC register.  If there is no PC specified in the ISA, 
  /// this returns a default name
  std::string GetPCName();

  /// Retrieves the register class of the PC.  If no register class is specified, then 
  /// this returns a null string
  std::string GetPCRegClass();
};

#endif

// EOF
