//
// _SCParser_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCParser_H_
#define _STONECUTTER_SCParser_H_

#include "CoreGen/StoneCutter/SCLexer.h"
#include "CoreGen/StoneCutter/SCAST.h"
#include "CoreGen/StoneCutter/SCMsg.h"

#include "llvm/ADT/STLExtras.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

class SCParser{
public:
  /// Default constructor
  SCParser(std::string, std::string, SCMsg *);

  /// Default destructor
  ~SCParser();

  /// Parse the input set
  bool Parse();

private:
  std::string InBuf;                    ///< StoneCutter input buffer
  std::string FileName;                 ///< StoneCutter file name
  SCMsg *Msgs;                          ///< StoneCutter message handler
  SCLexer *Lex;                         ///< StoneCutter Lexer

  std::map<std::string, int> BinopPrecedence;  ///< StoneCutter binary operand precedence

  // private functions
  void InitBinopPrecedence();
};

#endif

// EOF
