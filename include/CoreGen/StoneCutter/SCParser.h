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
  int CurTok;                           ///< StoneCutter current token
  std::string InBuf;                    ///< StoneCutter input buffer
  std::string FileName;                 ///< StoneCutter file name
  SCMsg *Msgs;                          ///< StoneCutter message handler
  SCLexer *Lex;                         ///< StoneCutter Lexer

  std::map<char, int> BinopPrecedence;  ///< StoneCutter binary operand precedence

  // private functions

  /// Get the next token
  int GetNextToken();

  /// Get the token precedence
  int GetTokPrecedence();

  /// Initialize the binary operand precedence
  void InitBinopPrecedence();

  /// Parse a number expression
  std::unique_ptr<ExprAST> ParseNumberExpr();

  /// Parse a paren expression '(' expression ')'
  std::unique_ptr<ExprAST> ParseParenExpr();

  /// Parse an identifer expression
  std::unique_ptr<ExprAST> ParseIdentifierExpr();

  /// Primary expression parser
  std::unique_ptr<ExprAST> ParsePrimary();

  /// Parse binary operation RHS
  std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                         std::unique_ptr<ExprAST> LHS);

  /// Parse expressions
  std::unique_ptr<ExprAST> ParseExpression();

  /// Parse prototypes
  std::unique_ptr<PrototypeAST> ParsePrototype();

  /// Parse register class definitions
  std::unique_ptr<RegClassAST> ParseRegClassDef();

  /// Parse definitions
  std::unique_ptr<FunctionAST> ParseDefinition();

  /// Parse register classes
  std::unique_ptr<RegClassAST> ParseRegClass();

  /// Parse top-level expression
  std::unique_ptr<FunctionAST> ParseTopLevelExpr();

  /// Parse extern prototypes
  std::unique_ptr<PrototypeAST> ParseExtern();

  /// Handles definition parsing
  void HandleDefinition();

  /// Handles extern parsing
  void HandleExtern();

  /// Handles register class definitions
  void HandleRegClass();

  /// Handles top-level expressions
  void HandleTopLevelExpression();

  /// Logs an error
  std::unique_ptr<ExprAST> LogError(std::string Str);

  /// Logs a prototype error
  std::unique_ptr<PrototypeAST> LogErrorP(std::string Str);

  /// Logs a register class error
  std::unique_ptr<RegClassAST> LogErrorR(std::string Str);
};

#endif

// EOF
