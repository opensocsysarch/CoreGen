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

// standard headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

// CoreGen headers
#include "CoreGen/StoneCutter/SCLexer.h"
#include "CoreGen/StoneCutter/SCMsg.h"
#include "CoreGen/StoneCutter/SCUtil.h"

// LLVM headers
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

using namespace llvm;


class SCParser{
public:

  /// Default constructor
  SCParser(std::string, std::string, SCMsg *);

  /// Overloaded constructor
  SCParser(SCMsg *);

  /// Default destructor
  ~SCParser();

  /// Parse the input set
  bool Parse();

  /// Sets the input buffer and input file names
  bool SetInputs( std::string, std::string );

  /// Execute the optimizer against the IR
  bool Optimize();

  // AST Classes

  /// ExprAST - Base class for all expression nodes.
  class ExprASTContainer {
    public:
      virtual ~ExprASTContainer() = default;

      virtual Value *codegen() = 0;
  };

  /// ForExprAST - Expression class for for/in.
  class ForExprASTContainer : public ExprASTContainer {
    std::string VarName;
    std::unique_ptr<ExprASTContainer> Start, End, Step, Body;

  public:
    ForExprASTContainer(const std::string &VarName,
                        std::unique_ptr<ExprASTContainer> Start,
                        std::unique_ptr<ExprASTContainer> End,
                        std::unique_ptr<ExprASTContainer> Step,
                        std::unique_ptr<ExprASTContainer> Body)
      : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
      Step(std::move(Step)), Body(std::move(Body)) {}

    Value *codegen() override;
  };

  /// IfExprAST - Expression class for conditionals
  class IfExprASTContainer : public ExprASTContainer {
    std::unique_ptr<ExprASTContainer> Cond, Then, Else;

    public:
      IfExprASTContainer(std::unique_ptr<ExprASTContainer> Cond,
                         std::unique_ptr<ExprASTContainer> Then,
                         std::unique_ptr<ExprASTContainer> Else)
        : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

      Value *codegen() override;
  };

  /// NumberExprAST - Expression class for numeric literals like "1.0".
  class NumberExprASTContainer : public ExprASTContainer {
    double Val;

  public:
    NumberExprASTContainer(double Val) : Val(Val) {}

    Value *codegen() override;
  };

  /// VariableExprAST - Expression class for referencing a variable, like "a".
  class VariableExprASTContainer : public ExprASTContainer {
    std::string Name;

  public:
    VariableExprASTContainer(const std::string &Name) : Name(Name) {}

    Value *codegen() override;
  };

  /// BinaryExprAST - Expression class for a binary operator.
  class BinaryExprASTContainer : public ExprASTContainer {
    char Op;
    std::unique_ptr<ExprASTContainer> LHS, RHS;

  public:
    BinaryExprASTContainer(char Op, std::unique_ptr<ExprASTContainer> LHS,
                           std::unique_ptr<ExprASTContainer> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    Value *codegen() override;
  };

  /// CallExprAST - Expression class for function calls.
  class CallExprASTContainer : public ExprASTContainer {
    std::string Callee;
    std::vector<std::unique_ptr<ExprASTContainer>> Args;

  public:
    CallExprASTContainer(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprASTContainer>> Args)
      : Callee(Callee), Args(std::move(Args)) {}

    Value *codegen() override;
  };

  /// RegClassAST - This class represents a register class definition
  /// which captures the name and the associated registers
  class RegClassASTContainer {
    std::string Name;
    std::vector<std::string> Args;
    std::vector<VarAttrs> Attrs;

  public:
    RegClassASTContainer(const std::string &Name,
                         std::vector<std::string> Args,
                         std::vector<VarAttrs> Attrs)
      : Name(Name), Args(std::move(Args)), Attrs(std::move(Attrs)) {}

    const std::string &getName() const { return Name; }
    Value *codegen();
  };


  /// PrototypeAST - This class represents the "prototype" for a function,
  /// which captures its name, and its argument names (thus implicitly the number
  /// of arguments the function takes).
  class PrototypeASTContainer {
    std::string Name;
    std::vector<std::string> Args;

  public:
    PrototypeASTContainer(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}

    Function *codegen();
    const std::string &getName() const { return Name; }
  };

  /// FunctionAST - This class represents a function definition itself.
  class FunctionASTContainer {
    std::unique_ptr<PrototypeASTContainer> Proto;
    std::unique_ptr<ExprASTContainer> Body;

  public:
    FunctionASTContainer(std::unique_ptr<PrototypeASTContainer> Proto,
                         std::unique_ptr<ExprASTContainer> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}

    Function *codegen();
  };

  // LLVM CodeGen Variables
  static LLVMContext TheContext;
  static IRBuilder<> Builder;
  static std::unique_ptr<Module> TheModule;
  static std::map<std::string, Value *> NamedValues;
  static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

private:

  int CurTok;                           ///< StoneCutter current token
  std::string InBuf;                    ///< StoneCutter input buffer
  std::string FileName;                 ///< StoneCutter file name
  SCMsg *Msgs;                          ///< StoneCutter message handler
  SCLexer *Lex;                         ///< StoneCutter Lexer

  // flags
  bool InFunc;                          ///< Determines whether the parser is in a function body
  bool IsOpt;                           ///< Determines whether to run the optimizer

  bool Rtn;                             ///< Return status of the parser

  std::map<char, int> BinopPrecedence;  ///< StoneCutter binary operand precedence

  // private functions

  /// Initialize the pass manager
  void InitModuleandPassManager();

  /// Get the next token
  int GetNextToken();

  /// Parse the variable definition and return the complementary VarAttr
  bool GetVarAttr( std::string Str, VarAttrs &V );

  /// Get the token precedence
  int GetTokPrecedence();

  /// Initialize the binary operand precedence
  void InitBinopPrecedence();

  /// Parse a number expression
  std::unique_ptr<ExprASTContainer> ParseNumberExpr();

  /// Parse a paren expression '(' expression ')'
  std::unique_ptr<ExprASTContainer> ParseParenExpr();

  /// Parse an identifer expression
  std::unique_ptr<ExprASTContainer> ParseIdentifierExpr();

  /// Primary expression parser
  std::unique_ptr<ExprASTContainer> ParsePrimary();

  /// Parse binary operation RHS
  std::unique_ptr<ExprASTContainer> ParseBinOpRHS(int ExprPrec,
                                         std::unique_ptr<ExprASTContainer> LHS);

  /// Parse expressions
  std::unique_ptr<ExprASTContainer> ParseExpression();

  /// Parse conditional expressions
  std::unique_ptr<ExprASTContainer> ParseIfExpr();

  /// Parse prototypes
  std::unique_ptr<PrototypeASTContainer> ParsePrototype();

  /// Parse register class definitions
  std::unique_ptr<RegClassASTContainer> ParseRegClassDef();

  /// Parse definitions
  std::unique_ptr<FunctionASTContainer> ParseDefinition();

  /// Parse register classes
  std::unique_ptr<RegClassASTContainer> ParseRegClass();

  /// Parse top-level expression
  std::unique_ptr<FunctionASTContainer> ParseTopLevelExpr();

  /// Parse extern prototypes
  std::unique_ptr<PrototypeASTContainer> ParseExtern();

  /// Parse a for loop
  std::unique_ptr<ExprASTContainer> ParseForExpr();

  /// Parse the closing of a function body
  bool ParseCloseBracket();

  /// Handles definition parsing
  void HandleDefinition();

  /// Handles extern parsing
  void HandleExtern();

  /// Handles register class definitions
  void HandleRegClass();

  /// Handles top-level expressions
  void HandleTopLevelExpression();

  // Handles function closing
  void HandleFuncClose();

  /// Logs an error
  std::unique_ptr<ExprASTContainer> LogError(std::string Str);

  /// Logs a prototype error
  std::unique_ptr<PrototypeASTContainer> LogErrorP(std::string Str);

  /// Logs a register class error
  std::unique_ptr<RegClassASTContainer> LogErrorR(std::string Str);

  /// Logs a function error
  std::unique_ptr<FunctionASTContainer> LogErrorF(std::string Str);

  /// Logs a value error
};

Value *LogErrorV(std::string Str);  // FIX THIS

typedef SCParser::ExprASTContainer ExprAST;
typedef SCParser::NumberExprASTContainer NumberExprAST;
typedef SCParser::VariableExprASTContainer VariableExprAST;
typedef SCParser::BinaryExprASTContainer BinaryExprAST;
typedef SCParser::CallExprASTContainer CallExprAST;
typedef SCParser::PrototypeASTContainer PrototypeAST;
typedef SCParser::FunctionASTContainer FunctionAST;
typedef SCParser::RegClassASTContainer RegClassAST;
typedef SCParser::IfExprASTContainer IfExprAST;
typedef SCParser::ForExprASTContainer ForExprAST;

#endif

// EOF