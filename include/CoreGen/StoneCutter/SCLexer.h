//
// _SCLexer_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCLEXER_H_
#define _STONECUTTER_SCLEXER_H_

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

// StoneCutter headers
#include "CoreGen/StoneCutter/SCUtil.h"

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


enum SCToken {
  // end of file
  tok_eof       = -1,

  // commands
  tok_def       = -2,
  tok_inst      = -3,
  tok_extern    = -4,
  tok_regclass  = -5,

  // primary
  tok_identifier = -6,
  tok_number     = -7,

  // conditionals
  tok_if          = -8,
  tok_elseif      = -9,
  tok_else        = -10,

  // loop control
  tok_for         = -11,
  tok_var         = -12
};

class SCLexer{
public:
  /// Standard constructor
  SCLexer(std::string B);

  /// Standard destructor
  ~SCLexer();

  /// Return the next token from the input stream
  int GetTok();

  /// Retrieve the identifier string
  std::string GetIdentifierStr() { return IdentifierStr; }

  /// Retrieve the immediate numeric value
  double GetNumVal() { return NumVal; }

  /// Retrieve the line number
  unsigned long GetLineNum() { return LineNum; }

  /// Retrieve the most recent variable entry data
  VarAttrs GetVarAttrs() { return Var; }

private:
  std::string InBuf;                ///< Input buffer
  std::string IdentifierStr;        ///< Utilized for tok_identifier
  double NumVal;                    ///< Utilized for tok_number
  unsigned long LineNum;            ///< Current line number
  int CurChar;                      ///< Current parsed character
  VarAttrs Var;                     ///< Parameters for most recent variable entry

  /// Read the next character from the input string
  int GetNext();

  /// Determine if the current token is a variable definition
  bool IsVarDef();
};

#endif

// EOF
