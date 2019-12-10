//
// _SCExec_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCExec
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter top-level execution class
 *
 */

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
#include <fstream>
#include <sstream>
#include <ctime>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <streambuf>

// StoneCutter Headers
#include "CoreGen/StoneCutter/SCOpts.h"
#include "CoreGen/StoneCutter/SCMsg.h"
#include "CoreGen/StoneCutter/SCParser.h"
#include "CoreGen/StoneCutter/SCLLCodeGen.h"
#include "CoreGen/StoneCutter/SCChiselCodeGen.h"
#include "CoreGen/StoneCutter/SCUtil.h"

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

class SCExec{
private:
  // private variables
  SCOpts *Opts;       ///< SCExec: Options object
  SCMsg *Msgs;        ///< SCExec: Messages object
  SCParser *Parser;   ///< SCExec: Parser object

  // private functions
  /// SCExec: Print the list of passes
  bool PrintPassList();

public:
  /// SCExec: Constructor
  SCExec(SCOpts *, SCMsg *);

  /// SCExec: Destructor
  ~SCExec();

  /// SCExec: Execute the compiler with the given options
  bool Exec();
};

#endif // _STONECUTTER_SCEXEC_H_
