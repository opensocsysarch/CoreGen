//
// _SCParser_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCParser
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter language parsing class
 *
 */

#ifndef _STONECUTTER_SCParser_H_
#define _STONECUTTER_SCParser_H_

// standard headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <utility>
#include <tuple>
#include <memory>
#include <string>
#include <vector>

// CoreGen headers
#include "CoreGen/StoneCutter/SCLexer.h"
#include "CoreGen/StoneCutter/SCMsg.h"
#include "CoreGen/StoneCutter/SCUtil.h"
#include "CoreGen/StoneCutter/SCIntrinsics.h"

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

/** SCDyad: enumerated types to describe dyadic operators */
enum SCDyad {
  dyad_shfl   = 10,   ///< SCDyad: Shift left '<<'
  dyad_shfr   = 11,   ///< SCDyad: Shift right '>>'
  dyad_eqeq   = 12,   ///< SCDyad: Boolean equal '=='
  dyad_noteq  = 13,   ///< SCDyad: Boolean not equal '!='
  dyad_logand = 14,   ///< SCDyad: Boolean and '&&'
  dyad_logor  = 15,   ///< SCDyad: Boolean or '||'
  dyad_gte    = 16,   ///< SCDyad: Greater than or equal to '>='
  dyad_lte    = 17    ///< SCDyad: Less than or equal to '>='
};

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

  /// Disables individual passes; all others enabled
  bool DisablePasses(std::vector<std::string> P);

  /// Enables individual passes; all others disabled
  bool EnablePasses(std::vector<std::string> P);

  /// Retrieve the list of LLVM passes
  std::vector<std::string> GetPassList();

  // AST Classes

  /// ExprAST - Base class for all expression nodes.
  class ExprASTContainer {
    public:
      /// Virtualized base expression container function
      virtual ~ExprASTContainer() = default;

      /// Virtualized base expression code generation function
      virtual Value *codegen() = 0;
  };

  /// ForExprAST - Expression class for for/in.
  class ForExprASTContainer : public ExprASTContainer {
    std::string VarName;                            ///< Variable name
    std::unique_ptr<ExprASTContainer> Start;        ///< Start of an expression body
    std::unique_ptr<ExprASTContainer> End;          ///< End of an expression body
    std::unique_ptr<ExprASTContainer> Step;         ///< Step function for an expression body
    std::unique_ptr<ExprASTContainer> Body;         ///< Expression body
    std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr;  ///< Vector of body expressions

  public:
    /// ForExprASTContainer default constructor
    ForExprASTContainer(const std::string &VarName,
                        std::unique_ptr<ExprASTContainer> Start,
                        std::unique_ptr<ExprASTContainer> End,
                        std::unique_ptr<ExprASTContainer> Step,
                        std::unique_ptr<ExprASTContainer> Body)
      : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
      Step(std::move(Step)), Body(std::move(Body)) {}

    /// ForExprASTContainer overloaded constructor
    ForExprASTContainer(const std::string &VarName,
                        std::unique_ptr<ExprASTContainer> Start,
                        std::unique_ptr<ExprASTContainer> End,
                        std::unique_ptr<ExprASTContainer> Step,
                        std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr)
      : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
      Step(std::move(Step)), BodyExpr(std::move(BodyExpr)) {}

    /// ForExprASTContainer code generation driver
    Value *codegen() override;
  };

  ///VarExprAST - Expression class for variables
  class VarExprASTContainer : public ExprASTContainer {
    std::vector<std::pair<std::string,
                          std::unique_ptr<ExprASTContainer>>> VarNames; ///< Vector of variable names
    std::vector<VarAttrs> Attrs;    ///< Vector of variable attributes
    std::unique_ptr<ExprASTContainer> Body;   ///< Expression container body
  public:
    /// VarExprASTContainer default constructor
    VarExprASTContainer(std::vector<std::pair<std::string,
                                      std::unique_ptr<ExprASTContainer>>> VarNames,
                          std::vector<VarAttrs> Attrs,
                          std::unique_ptr<ExprASTContainer> Body)
        : VarNames(std::move(VarNames)),
          Attrs(std::move(Attrs)),
          Body(std::move(Body)) {}

    /// VarExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// IfExprAST - Expression class for conditionals
  class IfExprASTContainer : public ExprASTContainer {
    std::unique_ptr<ExprASTContainer> Cond;   ///< Conditional expression container
    std::unique_ptr<ExprASTContainer> Then;   ///< Then expression container
    std::unique_ptr<ExprASTContainer> Else;   ///< Else expression container
  public:
    /// IfExprASTContainer default constructor
    IfExprASTContainer(std::unique_ptr<ExprASTContainer> Cond,
                         std::unique_ptr<ExprASTContainer> Then,
                         std::unique_ptr<ExprASTContainer> Else)
        : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

    /// IfExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// NumberExprAST - Expression class for numeric literals like "1.0".
  class NumberExprASTContainer : public ExprASTContainer {
    uint64_t Val;     ///< Value of the immediate

  public:
    /// NumberExprASTContainer default constructor
    NumberExprASTContainer(uint64_t Val) : Val(Val) {}

    /// NumberExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// VariableExprAST - Expression class for referencing a variable, like "a".
  class VariableExprASTContainer : public ExprASTContainer {
    std::string Name;   ///< Variable name

  public:
    /// VariableExprASTContainer default constructor
    VariableExprASTContainer(const std::string &Name) : Name(Name) {}

    /// VariableExprASTContainer code generation driver
    Value *codegen() override;

    /// VariableExprASTContainer: retrieve the name of the variable
    const std::string &getName() const { return Name; }
  };

  /// BinaryExprAST - Expression class for a binary operator.
  class BinaryExprASTContainer : public ExprASTContainer {
    char Op;    ///< Binary operator
    std::unique_ptr<ExprASTContainer> LHS;    ///< Left hand side of the expression
    std::unique_ptr<ExprASTContainer> RHS;    ///< Right hand side of the expression

  public:
    /// BinaryExprASTContainer default constructor
    BinaryExprASTContainer(char Op, std::unique_ptr<ExprASTContainer> LHS,
                           std::unique_ptr<ExprASTContainer> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    /// BinaryExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// CallExprAST - Expression class for function calls.
  class CallExprASTContainer : public ExprASTContainer {
    std::string Callee;   ///< Callee of the function
    std::vector<std::unique_ptr<ExprASTContainer>> Args; ///< Arguments to the function
    bool Intrin;    ///<  determines whether the call is an intrinsic

  public:
    /// CallExprASTContainer default constructor
    CallExprASTContainer(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprASTContainer>> Args,
                         bool Intrin)
      : Callee(Callee), Args(std::move(Args)), Intrin(Intrin) {}

    /// CallExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// RegClassAST - This class represents a register class definition
  ///               which captures the name and the associated registers
  class RegClassASTContainer {
    std::string Name;     ///< Name of the register class
    std::vector<std::string> Args;  ///< Register vector
    std::vector<VarAttrs> Attrs;    ///< Register attribute vector
    std::vector<std::tuple<std::string,std::string,VarAttrs>> SubRegs;  ///< Subregister vector

  public:
    /// RegClassASTContainer default constructor
    RegClassASTContainer(const std::string &Name,
                         std::vector<std::string> Args,
                         std::vector<VarAttrs> Attrs,
                         std::vector<std::tuple<std::string,std::string,VarAttrs>> SubRegs)
      : Name(Name), Args(std::move(Args)),
        Attrs(std::move(Attrs)),SubRegs(std::move(SubRegs)) {}

    /// RegClassASTContainer: Retrieve the  name of the register class
    const std::string &getName() const { return Name; }

    /// RegClassASTContainer code generation driver
    Value *codegen();
  };


  /// PrototypeAST - This class represents the "prototype" for a function,
  /// which captures its name, and its argument names (thus implicitly the number
  /// of arguments the function takes).
  class PrototypeASTContainer {
    std::string Name;               ///< Name of the instruction protoype
    std::vector<std::string> Args;  ///< Argument vector of the prototype

  public:
    /// PrototypeASTContainer default constructor
    PrototypeASTContainer(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}

    /// PrototypeASTContainer code generation driver
    Function *codegen();

    /// PrototypeASTContainer: retrieve the name of the instruction definition
    const std::string &getName() const { return Name; }
  };

  /// FunctionAST - This class represents a function definition itself.
  class FunctionASTContainer {
    std::unique_ptr<PrototypeASTContainer> Proto;       ///< Prototype for this function
    std::vector<std::unique_ptr<ExprASTContainer>> Body;///< Vector of body expressions

  public:
    /// FunctionASTContainer default constructor
    FunctionASTContainer(std::unique_ptr<PrototypeASTContainer> Proto,
                         std::vector<std::unique_ptr<ExprASTContainer>> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}

    /// FunctionASTContainer code generation driver
    Function *codegen();
  };

  // LLVM CodeGen Variables
  static LLVMContext TheContext;        ///< LLVM context
  static IRBuilder<> Builder;           ///< LLVM IR Builder
  static std::unique_ptr<Module> TheModule; ///< LLVM top-level Module
  static std::map<std::string, AllocaInst*> NamedValues;  ///< Map of named values in scope
  static std::map<std::string, GlobalVariable*> GlobalNamedValues;  ///< map of global values always in scope
  static std::map<std::string, std::unique_ptr<PrototypeASTContainer>> FunctionProtos;  ///< map of function prototypes
  static std::unique_ptr<legacy::FunctionPassManager> TheFPM; ///< LLVM function pass manager
  static unsigned LabelIncr;  ///< Label incrementer
  static bool IsOpt;          ///< Are optimizations enabled?
  static SCMsg *GMsgs;        ///< Global message handler

private:

  int CurTok;                           ///< StoneCutter current token
  std::string InBuf;                    ///< StoneCutter input buffer
  std::string FileName;                 ///< StoneCutter file name
  SCMsg *Msgs;                          ///< StoneCutter message handler
  SCLexer *Lex;                         ///< StoneCutter Lexer

  // flags
  bool InFunc;                          ///< Determines whether the parser is in a function body

  bool Rtn;                             ///< Return status of the parser

  std::map<char, int> BinopPrecedence;  ///< StoneCutter binary operand precedence

  std::map<std::string,bool> EPasses;   ///< LLVM enabled passes

  std::vector<SCIntrin *> Intrins;      ///< StoneCutter Intrinsics

  // private functions

  /// Inserts all the necessary intrinsic externs into the input stream
  bool InsertExternIntrin();

  /// Checks the call expression and determines if it is an intrinsci
  bool CheckIntrinName(std::string Name);

  /// Get the number of required arguments for the target intrinsic
  unsigned GetNumIntrinArgs(std::string Name);

  /// Checks the input vector of pass names against the known passes list
  bool CheckPassNames(std::vector<std::string> P);

  /// Calculates the edit distance between the two strings
  unsigned EditDistance(std::string &s1, std::string &s2);

  /// Retrieves the closest string to the input pass name
  std::string GetNearbyString(std::string &Input);

  /// Initializes the pass map
  void InitPassMap();

  /// Initializes the intrinsic vector
  void InitIntrinsics();

  /// Determines whether the target pass is enabled
  bool IsPassEnabled(std::string P);

  /// Initialize the pass manager
  void InitModuleandPassManager();

  /// Get the next token
  int GetNextToken();

  /// Parse the variable definition and return the complementary VarAttr
  bool GetVarAttr( std::string Str, VarAttrs &V );

  /// Get the token precedence
  int GetTokPrecedence();

  /// Convert the identifier string to a dyadic operator value
  int StrToDyad();

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

  /// Parse a variable expression
  std::unique_ptr<ExprASTContainer> ParseVarExpr();

  /// Parse the closing of a function body
  bool ParseCloseBracket();

  /// Retrieves the largest VarAttr from a VarAttr vector
  VarAttrs GetMaxVarAttr(std::vector<VarAttrs> ArgAttrs);

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

/** Typedef for ExprASTContainer */
typedef SCParser::ExprASTContainer ExprAST;
/** Typedef for NumerExprASTContainer */
typedef SCParser::NumberExprASTContainer NumberExprAST;
/** Typedef for VariableExprASTContainer */
typedef SCParser::VariableExprASTContainer VariableExprAST;
/** Typedef for BinaryExprASTContainer */
typedef SCParser::BinaryExprASTContainer BinaryExprAST;
/** Typedef for CallExprASTContainer */
typedef SCParser::CallExprASTContainer CallExprAST;
/** Typedef for PrototypeASTContainer */
typedef SCParser::PrototypeASTContainer PrototypeAST;
/** Typedef for FunctionASTContainer */
typedef SCParser::FunctionASTContainer FunctionAST;
/** Typedef for RegClassASTContainer */
typedef SCParser::RegClassASTContainer RegClassAST;
/** Typedef for IfExprASTContainer */
typedef SCParser::IfExprASTContainer IfExprAST;
/** Typedef for ForExprASTContainer */
typedef SCParser::ForExprASTContainer ForExprAST;
/** Typedef for VarExprASTContainer */
typedef SCParser::VarExprASTContainer VarExprAST;

#endif

// EOF
