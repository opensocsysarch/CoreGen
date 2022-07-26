//
// _SCParser_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
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
#include <numeric>
#include <string>

// CoreGen headers
#include "CoreGen/StoneCutter/SCLexer.h"
#include "CoreGen/StoneCutter/SCMsg.h"
#include "CoreGen/StoneCutter/SCOpts.h"
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

#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/PromoteMemToReg.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"

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

/** SCInstField: enumerated types to describe instruction fields */
typedef enum {
  field_enc = 100,    ///< SCInstField: Encoding field
  field_reg = 101,    ///< SCInstField: Register field
  field_imm = 102,    ///< SCInstField: Immediate field
  field_unk = 200     ///< SCInstField: Unknown field type
}SCInstField;

class SCParser{
public:

  /// Default constructor
  SCParser(std::string, std::string, SCOpts *, SCMsg *);

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

  /// Enables all LLVM passes
  void EnableAllPasses();

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

  /// DoWhileExprAST - Expression class for do while loops
  class DoWhileExprASTContainer : public ExprASTContainer {
    std::unique_ptr<ExprASTContainer> Cond;         ///< Conditional expression body
    std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr;  ///< Vector of body expressions

  public:
    /// DoWhileExprASTContainer default constructor
    DoWhileExprASTContainer(std::unique_ptr<ExprASTContainer> Cond,
                          std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr)
      : Cond(std::move(Cond)), BodyExpr(std::move(BodyExpr)) {}

    /// DoWhileExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// PipeExprAST - Expression class for pipe regions
  class PipeExprASTContainer : public ExprASTContainer {
    std::string PipeName;                                     ///< Name of the target pipe stage
    std::string PipeLine;                                     ///< Pipeline designator
    unsigned Instance;                                        ///< Instance of the pipe stage
    std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr;  ///< Vector of body expressions

  public:
    /// PipExprASTContainer default constructor
    PipeExprASTContainer(std::string PipeName,
                         std::string PipeLine,
                         unsigned Instance,
                         std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr)
      : PipeName(PipeName), PipeLine(PipeLine), Instance(Instance),
        BodyExpr(std::move(BodyExpr)) {}

    /// PipeExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// WhileExprAST - Expression class for while loops
  class WhileExprASTContainer : public ExprASTContainer {
    std::unique_ptr<ExprASTContainer> Cond;         ///< Conditional expression body
    std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr;  ///< Vector of body expressions

  public:
    /// WhileExprASTContainer default contstructor
    WhileExprASTContainer(std::unique_ptr<ExprASTContainer> Cond,
                          std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr)
      : Cond(std::move(Cond)), BodyExpr(std::move(BodyExpr)) {}

    /// WhileExprASTContainer code generation driver
    Value *codegen() override;
  };

  /// ForExprAST - Expression class for for loops
  class ForExprASTContainer : public ExprASTContainer {
    std::string VarName;                            ///< Variable name
    std::unique_ptr<ExprASTContainer> Start;        ///< Start of an expression body
    std::unique_ptr<ExprASTContainer> End;          ///< End of an expression body
    std::unique_ptr<ExprASTContainer> Step;         ///< Step function for an expression body
    std::vector<std::unique_ptr<ExprASTContainer>> BodyExpr;  ///< Vector of body expressions

  public:
    /// ForExprASTContainer default constructor
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

  /// VarExprAST - Expression class for variables
  class VarExprASTContainer : public ExprASTContainer {
    std::vector<std::pair<std::string,
                          std::unique_ptr<ExprASTContainer>>> VarNames; ///< Vector of variable names
    std::vector<VarAttrs> Attrs;              ///< Vector of variable attributes
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
    std::vector<std::unique_ptr<ExprASTContainer>> ThenV;  ///< Vector of then body expressions
    std::vector<std::unique_ptr<ExprASTContainer>> ElseV;  ///< Vector of else body expressions

  public:
    /// IfExprASTContainer default constructor
    IfExprASTContainer(std::unique_ptr<ExprASTContainer> Cond,
                       std::vector<std::unique_ptr<ExprASTContainer>> ThenExpr,
                       std::vector<std::unique_ptr<ExprASTContainer>> ElseExpr)
      : Cond(std::move(Cond)), ThenV(std::move(ThenExpr)), ElseV(std::move(ElseExpr)) {}

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
    // std::vector<std::tuple<std::string, std::string, unsigned, unsigned>> LocalContainers; ///< vector of all instances of mat and vec types 
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
    std::vector<std::string> OrigNames; ///< Original argument names (unmangled)
    bool Intrin;    ///<  determines whether the call is an intrinsic

  public:
    /// CallExprASTContainer default constructor
    CallExprASTContainer(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprASTContainer>> Args,
                         std::vector<std::string> OrigNames,
                         bool Intrin)
      : Callee(Callee), Args(std::move(Args)), OrigNames(std::move(OrigNames)),
        Intrin(Intrin) {}

    /// CallExprASTContainer code generation driver
    Value *codegen() override;

    /// CallExprASTContainer: determines whether the call is a StoneCutter intrinsic
    bool isIntrin() { return Intrin; }

    /// CallExprASTContainer: retrieves the number of original argument names
    unsigned getNumOrigNames() { return OrigNames.size(); }

    /// CallExprASTContainer: retrieves the target original argument name
    std::string getOrigName(unsigned Idx){
      std::string Name;
      if( Idx > (OrigNames.size()-1) )
        return Name;
      return OrigNames[Idx];
    }
  };

  /// InstFormatASTContainer - This class represents an instruction format definition
  ///                          which captures the fields of an instruction format
  class InstFormatASTContainer {
    std::string Name;   ///< Name of the instruction format
    std::vector<std::tuple<std::string,
                           SCInstField,
                           std::string,
                           unsigned>> Fields; ///< Field vector

  public:
    /// InstFormatASTContainer default constructor
    InstFormatASTContainer(const std::string &Name,
                           std::vector<std::tuple<std::string,
                                                  SCInstField,
                                                  std::string,
                                                  unsigned>> Fields)
      : Name(Name), Fields(std::move(Fields)) {}

    /// InstFormatASTContainer: Retrieve the name of an instruction format
    const std::string &getName() const { return Name; }

    /// InstFormatASTContainer code generation driver
    Value *codegen();
  };

  /// PipelineASTContainer - This class represents a pipeline definition with
  ///               its respective attribuets
  class PipelineASTContainer{
    std::string Name;                 ///< Name of the pipeline unit
    std::vector<std::string> Attrs;   ///< Pipeline attribute vector

  public:
    /// PipelineASTContainer default constructor
    PipelineASTContainer(const std::string &Name,
                         std::vector<std::string> Attrs)
      : Name(Name), Attrs(std::move(Attrs)) {}

    /// PipelineASTContainer: Retrieve the  name of the register class
    const std::string &getName() const { return Name; }

    /// PipelineASTContainer: code generation driver
    Value *codegen();
  };

  /// RegClassASTContainer - This class represents a register class definition
  ///               which captures the name and the associated registers
  class RegClassASTContainer {
    std::string Name;               ///< Name of the register class
    std::string PC;                 ///< Name of the PC register
    std::vector<std::string> Args;  ///< Register vector
    std::vector<VarAttrs> Attrs;    ///< Register attribute vector
    std::vector<std::tuple<std::string,std::string,VarAttrs>> SubRegs;  ///< Subregister vector

  public:
    /// RegClassASTContainer default constructor
    RegClassASTContainer(const std::string &Name,
                         const std::string &PC,
                         std::vector<std::string> Args,
                         std::vector<VarAttrs> Attrs,
                         std::vector<std::tuple<std::string,std::string,VarAttrs>> SubRegs)
      : Name(Name), PC(PC), Args(std::move(Args)),
        Attrs(std::move(Attrs)),SubRegs(std::move(SubRegs)) {}

    /// RegClassASTContainer: Retrieve the  name of the register class
    const std::string &getName() const { return Name; }

    /// RegClassASTContainer code generation driver
    Value *codegen();
  };


  /// PrototypeASTContainer - This class represents the "prototype" for a function,
  /// which captures its name, and its argument names (thus implicitly the number
  /// of arguments the function takes).
  class PrototypeASTContainer {
    std::string Name;               ///< Name of the instruction protoype
    std::string InstFormat;         ///< Name of the instruction format
    bool VLIW;                      ///< Is this prototype a VLIW
    unsigned Stage;                 ///< Pipeline stage for VLIW
    std::vector<std::string> Args;  ///< Argument vector of the prototype

  public:
    /// PrototypeASTContainer default constructor
    PrototypeASTContainer(const std::string &Name,
                          const std::string &IName,
                          bool VLIW,
                          unsigned Stage,
                          std::vector<std::string> Args)
        : Name(Name), InstFormat(IName), VLIW(VLIW), Stage(Stage),
          Args(std::move(Args)) {}

    /// PrototypeASTContainer: code generation driver
    Function *codegen();

    /// PrototypeASTContainer: retrieve the name of the instruction definition
    const std::string &getName() const { return Name; }

    /// PrototypeASTContainer: retrieve the instruction format
    const std::string &getInstFormat() const { return InstFormat; }
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
  static LLVMContext TheContext;                                                                ///< LLVM context
  static IRBuilder<> Builder;                                                                   ///< LLVM IR Builder
  static std::unique_ptr<Module> TheModule;                                                     ///< LLVM top-level Module
  static std::map<std::string, AllocaInst*> NamedValues;                                        ///< Map of named values in scope
  static std::map<std::string, GlobalVariable*> GlobalNamedValues;                              ///< map of global values always in scope
  static std::vector<std::tuple<std::string, std::string, unsigned, unsigned>> LocalContainers; ///< vector of all instances of mat and vec types 
  std::string CurrentFunction;                                                                  ///< current function name for tracking vecs and mats
  static std::map<std::string, std::unique_ptr<PrototypeASTContainer>> FunctionProtos;          ///< map of function prototypes
  static std::map<std::string,unsigned> PipeInstances;                                          ///< Pipe stage instance numbers
  static std::unique_ptr<legacy::FunctionPassManager> TheFPM;                                   ///< LLVM function pass manager
  static unsigned LabelIncr;                                                                    ///< Label incrementer
  static bool IsOpt;                                                                            ///< Are optimizations enabled?
  static bool IsPipe;                                                                           ///< Are we within a pipeline?
  static SCMsg *GMsgs;                                                                          ///< Global message handler
  static MDNode *NameMDNode;                                                                    ///< Pipe name metadata node
  static MDNode *InstanceMDNode;                                                                ///< Pipe instance metadata node
  static MDNode *PipelineMDNode;                                                                ///< Pipeline name metadata node

private:

  int CurTok;                           ///< StoneCutter current token
  std::string InBuf;                    ///< StoneCutter input buffer
  std::string FileName;                 ///< StoneCutter file name
  SCOpts *Opts;                         ///< Stonecutter options
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

  /// Checks the call expression and determines if it is an intrinsic
  bool CheckIntrinName(std::string Name);

  /// Checks the call expression and determines if it is an intrinsic
  static bool CheckLocalContainers(std::string FunctionName, std::string VarName, unsigned &DimX, unsigned &DimY);

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

  /// Parses VLIW stage numbering schemes
  bool ParseVLIWStage(std::string Name, unsigned &Stage);

  /// Parse the variable definition and return the complementary VarAttr
  bool GetVarAttr( std::string Str, VarAttrs &V );

  /// Parse the register field type and return the complementary SCInstField
  bool GetFieldAttr( std::string Str, SCInstField &F );

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

  /// Parse instructon field definitions
  std::unique_ptr<InstFormatASTContainer> ParseInstFormatDef();

  /// Parse definitions
  std::unique_ptr<FunctionASTContainer> ParseDefinition();

  /// Parse pipeline
  std::unique_ptr<PipelineASTContainer> ParsePipeline();

  /// Parse pipeline definition
  std::unique_ptr<PipelineASTContainer> ParsePipelineDef();

  /// Parse register classes
  std::unique_ptr<RegClassASTContainer> ParseRegClass();

  /// Parse instruction format
  std::unique_ptr<InstFormatASTContainer> ParseInstFormat();

  /// Parse top-level expression
  std::unique_ptr<FunctionASTContainer> ParseTopLevelExpr();

  /// Parse extern prototypes
  std::unique_ptr<PrototypeASTContainer> ParseExtern();

  /// Parse a for loop
  std::unique_ptr<ExprASTContainer> ParseForExpr();

  /// Parse a while loop
  std::unique_ptr<ExprASTContainer> ParseWhileExpr();

  /// Parse a do while loop
  std::unique_ptr<ExprASTContainer> ParseDoWhileExpr();

  /// Parse a variable expression
  std::unique_ptr<ExprASTContainer> ParseVarExpr();

  /// Parse a pipe expression
  std::unique_ptr<ExprASTContainer> ParsePipeExpr();

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

  /// Handles pipeline definitions
  void HandlePipeline();

  /// Handles instruction field definitions
  void HandleInstFormat();

  /// Handles top-level expressions
  void HandleTopLevelExpression();

  // Handles function closing
  void HandleFuncClose();

  /// Handles lexer errors
  void HandleLexerError();

  /// Error Code 
  SCError Errno;

  /// Retrieves the actual line number
  unsigned GetLineNum();


  /// Logs an error
  std::unique_ptr<ExprASTContainer> LogError(std::string Str);

  /// Logs a prototype error
  std::unique_ptr<PrototypeASTContainer> LogErrorP(std::string Str);

  /// Logs a register class error
  std::unique_ptr<RegClassASTContainer> LogErrorR(std::string Str);

  /// Logs a pipeline error
  std::unique_ptr<PipelineASTContainer> LogErrorPI(std::string Str);

  /// Logs an instruction format error
  std::unique_ptr<InstFormatASTContainer> LogErrorIF(std::string Str);

  /// Logs a function error
  std::unique_ptr<FunctionASTContainer> LogErrorF(std::string Str);

  /// Logs a value error
};

Value *LogErrorV(std::string Str);  // TODO: FIX THIS
Value *LogWarnV(std::string Str);  // TODO: FIX THIS

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
/** Typedef for PipelineASTContainer */
typedef SCParser::PipelineASTContainer PipelineAST;
/** Typedef for RegClassASTContainer */
typedef SCParser::RegClassASTContainer RegClassAST;
/** Typedef for InstFormatASTContainer */
typedef SCParser::InstFormatASTContainer InstFormatAST;
/** Typedef for IfExprASTContainer */
typedef SCParser::IfExprASTContainer IfExprAST;
/** Typedef for DoWhileExprASTContainer */
typedef SCParser::DoWhileExprASTContainer DoWhileExprAST;
/** Typedef for WhileExprASTContainer */
typedef SCParser::WhileExprASTContainer WhileExprAST;
/** Typedef for ForExprASTContainer */
typedef SCParser::ForExprASTContainer ForExprAST;
/** Typedef for VarExprASTContainer */
typedef SCParser::VarExprASTContainer VarExprAST;
/** Typedef for PipeExprASTContainer */
typedef SCParser::PipeExprASTContainer PipeExprAST;

#endif

// EOF
