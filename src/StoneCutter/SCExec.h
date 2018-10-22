//
// _SCExec_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCEXEC_H_
#define _STONECUTTER_SCEXEC_H_

#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

// StoneCutter Headers
#include "SCOpts.h"
#include "SCMsg.h"

// LLVM Headers
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

using namespace llvm;

/*! SCExec: Lexer Tokens */
enum Token {
  tok_eof = -1       // Token: End of File

  // commands

  // primary
};

class SCExec{
private:
  // private variables
  SCOpts *Opts;     /*! SCExec: Options object */
  SCMsg *Msgs;      /*! SCExec: Messages object */

  std::string IdentifierStr;  /*! SCExec: Filled in if tok_identifier */
  double NumVal;              /*! SCExec: Filled in if tok_number */
  int CurTok;                 /*! SCExec: Current token */

  std::map<char, int> BinopPrec;  /*! SCExec: Binary operand precedence */

  // private functions
  void initBinopPrecedence(void);

public:
  /*! SCExec: Constructor */
  SCExec(SCOpts *, SCMsg *);

  /*! SCExec: Destructor */
  ~SCExec();

  /*! SCExec: Execute the compiler with the given options */
  bool Exec();
};

#endif // _STONECUTTER_SCEXEC_H_
