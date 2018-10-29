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

#include "llvm/ADT/STLExtras.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

class SCLexer{
public:
  enum SCToken {
    // end of file
    tok_eof = -1,

    // commands
    tok_inst      = -2,
    tok_extern    = -3,
    tok_regclass  = -4,

    // primary
    tok_identifier = -5,
    tok_number     = -6
  };

  /// Standard constructor
  SCLexer(std::string B);

  /// Standard destructor
  ~SCLexer();

  /// Return the next token from the input stream
  int GetTok();

  /// Retrieve the identifier string
  std::string GetIdentifierStr() { return IdentifierStr; }

  /// Retrieve the immediate numeric value
  double GetVal() { return NumVal; }

  /// Retrieve the line number
  unsigned long GetLineNum() { return LineNum; }

private:
  std::string InBuf;                ///< Input buffer
  std::string IdentifierStr;        ///< Utilized for tok_identifier
  double NumVal;                    ///< Utilized for tok_number
  unsigned long LineNum;            ///< Current line number
};

#endif

// EOF
