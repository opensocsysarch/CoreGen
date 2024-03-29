//
// _DHDTGraph_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class DHDTGraph
 *
 * \ingroup DHDT
 *
 * \brief Deep Hardware Design Tool graph class
 *
 */

#ifndef _DHDTGRAPH_H_
#define _DHDTGRAPH_H_

// C++ Headers
#include <iostream>
#include <fstream>
#include <string>

// LLVM Headers
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
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
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/StoneCutter/SCIntrinsics.h"
#include "CoreGen/DHDT/DHDTInst.h"
#include "CoreGen/DHDT/DHDTConfig.h"
#include "CoreGen/DHDT/DHDTInst.h"

using namespace llvm;

/// DHDTNodeType: DHDT Node Types
typedef enum{
  // Unknown (default)
  DNodeUnk      = 0,          ///< DHDTNodeType: Unknown node

  // Top-level nodes
  DNodeTop      = 1,          ///< DHDTNodeType: Top-level node (crack/decode)

  // Function Nodes
  DNodeInst     = 2,          ///< DHDTNodeType: Instruction node
  DNodeVLIWPipe = 3,          ///< DHDTNodeType: VLIW Pipe node

  // Memory nodes
  DNodeRegRead  = 10,         ///< DHDTNodeType: Register read
  DNodeRegWrite = 11,         ///< DHDTNodeType: Register write
  DNodeMemRead  = 12,         ///< DHDTNodeType: Memory read
  DNodeMemWrite = 13,         ///< DHDTNodeType: Memory write
  DNodeFence    = 14,         ///< DHDTNodeType: Memory fence

  // ALU nodes
  DNodeArith    = 20,         ///< DHDTNodeType: Arithmetic node
  DNodeMux      = 21,         ///< DHDTNodeType: Mux node
  DNodeCond     = 22,         ///< DHDTNodeType: Cond branch

}DHDTNodeType;

/// DHDTLinkType: DHDT Link Types
typedef enum{
  // Scalar Links
  LNodeScalar   = 0,          ///< DHDTLinkType: Scalar links

  // VLIW Links
  LNodeCtrlIn   = 10,         ///< DHDTLinkType: Control input links
  LNodeCtrlOut  = 11,         ///< DHDTLinkType: Control output links
  LNodeDataIn   = 12,         ///< DHDTLinkType: Data input links
  LNodeDataOut  = 13          ///< DHDTLinkType: Data output links
}DHDTLinkType;

// Forward declarations
class DHDTNode;

class DHDTLink{
private:
  std::string Name;       ///< DHDTLink: name of the link
  DHDTLinkType Type;      ///< DHDTLink: type of the link
  unsigned Width;         ///< DHDTLink: width of the link (in bits)
  DHDTNode *Target;       ///< DHDTLink: target node

  double AVal;            ///< DHDTLink: accumulated power value

  DHDTConfig::ConfigEntry Entry;  ///< DHDTLink: power configuration


public:
  /// DHDTLink: constructor
  DHDTLink(std::string N, DHDTLinkType L)
    : Name(N), Type(L), AVal(0.) {}

  /// DHDTLink: overloaded constructor
  DHDTLink(std::string N, DHDTLinkType L, unsigned W)
    : Name(N), Type(L), Width(W), AVal(0.) {}

  /// DHDTLink: overloaded constructor
  DHDTLink(std::string N, DHDTLinkType L, DHDTNode *T)
    : Name(N), Type(L), Target(T), AVal(0.) {}

  /// DHDTLink: overloaded constructor
  DHDTLink(std::string N, DHDTLinkType L, unsigned W, DHDTNode *T)
    : Name(N), Type(L), Width(W), Target(T), AVal(0.) {}

  /// DHDTLink: destructor
  ~DHDTLink() {}

  /// DHDTLink: Get the name of the link
  std::string GetName() { return Name; }

  /// DHDTLink: Get the type of the link
  DHDTLinkType GetType() { return Type; }

  /// DHDTLink: Get the width of the link
  unsigned GetWidth() { return Width; }

  /// DHDTLink: Get the target node
  DHDTNode *GetTarget() { return Target; }

  /// DHDTLink: Set the power config entry
  void SetEntry(DHDTConfig::ConfigEntry E){ Entry = E; }

  /// DHDTLink: add the power for the accumulated total
  void AccumPower(double P){ AVal += P; }

  /// DHDTLink: retrieve the total accumulated power
  double GetTotalPower() { return AVal; }

  /// DHDTLink: Get config entry
  DHDTConfig::ConfigEntry GetEntry() { return Entry; }
};

class DHDTNode{
private:
  std::string Name;                 ///< DHDTNode: name of the node
  DHDTNodeType Type;                ///< DHDTNode: type of the node
  unsigned Width;                   ///< DHDTNode: width of the operation

  double AVal;                      ///< DHDTNode: accumulated power value

  DHDTConfig::ConfigEntry Entry;    ///< DHDTNode: power configuration

  std::vector<DHDTLink *> Links;    ///< DHDTnode: link vector

public:
  /// DHDTNode: constructor
  DHDTNode(std::string N, DHDTNodeType T)
    : Name(N), Type(T), Width(1), AVal(0.) {}

  /// DHDNode: overloaded constructor
  DHDTNode(std::string N, DHDTNodeType T, unsigned W)
    : Name(N), Type(T), Width(W), AVal(0.) {}

  /// DHDTNode: destructor
  ~DHDTNode() {}

  /// DHDTNode: retrieve the name of the node
  std::string GetName() { return Name; }

  /// DHDTNode: retrieve the type of the node
  DHDTNodeType GetType() { return Type; }

  /// DHDTNode: Insert link
  void InsertLink(DHDTLink *L) { Links.push_back(L); }

  /// DHDTNode: Get the number of links
  unsigned GetNumLinks() { return Links.size(); }

  /// DHDTNode: Get the width of the operation
  unsigned GetWidth() { return Width; }

  /// DHDTNode: Retrieve the links vector
  std::vector<DHDTLink *> GetLinks() { return Links; }

  /// DHDTNode: Set the power config entry
  void SetEntry(DHDTConfig::ConfigEntry E){ Entry = E; }

  /// DHDTNode: add the power for the accumulated total
  void AccumPower(double P){ AVal += P; }

  /// DHDTNode: retrieve the total accumulated power
  double GetTotalPower() { return AVal; }

  /// DHDTNode: Get config entry
  DHDTConfig::ConfigEntry GetEntry() { return Entry; }
};

class DHDTGraph{
private:
  // private variables
  bool Verbose;                   ///< DHDTGraph: Verbosity
  SMDiagnostic Err;               ///< DHDTGraph: LLVM diagnostic errors
  std::unique_ptr<Module> Mod;    ///< DHDTGraph: LLVM module pointer
  LLVMContext Context;            ///< DHDTGraph: LLVM Context
  CoreGenBackend CG;              ///< DHDTGraph: CoreGen Infrastructure
  DHDTConfig Config;              ///< DHDTGraph: DHDT Power Configuration

  std::vector<DHDTNode *> Nodes;  ///< DHDTGraph: DHDT Node vector
  std::vector<DHDTLink *> Links;  ///< DHDTGraph: DHDT Link vector

  std::map<std::string,DHDTNode *> NodeMap; ///< DHDTGraph: Map of function blocks to node names

  static std::vector<SCIntrin *> Intrins;  ///< DHDTGraph: Intrinsics

  /// DHDTGraph: Read the LLVM IR into graph form
  bool ReadLLVMIR(std::string IR);

  /// DHDTGraph: Read the CoreGen design file
  bool ReadCoreGenIR(std::string IR);

  /// DHDTGraph: build the LLVM graph
  bool BuildLLVMGraph();

  /// DHDTGraph: retrieve the top node
  DHDTNode *GetTop() { return Nodes[0]; }

  /// DHDTGraph: retrieve the target function node
  DHDTNode *GetFuncNode(std::string N);

  /// DHDTGraph: retrieve the latest DHDT node
  DHDTNode *GetLatestNode();

  /// DHDTGraph: retrieve the latest DHDT link
  DHDTLink *GetLatestLink();

  /// DHDTGraph: Check the VLIW function and determine if it is a top-level stage
  bool IsTopVLIWStage(Function &Func);

  /// DHDTGraph: Get the width of the store instruction operand
  unsigned GetStoreWidth(Instruction &Inst);

  /// DHDTGraph: Add a call node using the appropriate intrinsic
  bool AddCallNode(Instruction &Inst, DHDTNode *CurNode);

  /// DHDTGraph: Add a function node
  bool AddFunctionNode(Function &Func, unsigned &Stage);

  /// DHDTGraph: Add an instruction node
  bool AddInstNode(Function &Func, Instruction &Inst, DHDTNode *CurNode);

  /// DHDTGraph: Build the In/Out intrinsics
  bool HandleInOutIntrin();

  /// DHDTGraph: Find a matching function (VLIW Stage) for the target output signal vectors
  bool FindFuncMatch(DHDTNode *Parent,
                     std::vector<std::string> Outs,
                     std::vector<std::string> Types);

  /// DHDTGraph: Initialize all the internal link power configuration data
  bool InitLinkPower();

  /// DHDTGraph: converts the LLVM instruction opcode to a DHDT power config entry
  DHDTConfig::ConfigEntry LLVMInstToConfigType(unsigned Inst);

  /// DHDTGraph: converts an intrinsic name to the appropriate DHDT power config entry
  DHDTConfig::ConfigEntry IntrinToConfigType(std::string Intrin);

  /// DHDTGraph: Recursively execute the graph
  double RecurseGraph(DHDTNode *Node);

  /// DHDTGraph: Execute the graph from the target root node
  bool ExecuteGraph(DHDTNode *Root, double &Power);

  /// DHDTGraph: Execute the hazard graph and accumulate the traversed nodes from the target root
  bool ExecuteHazardGraph(DHDTNode *Root, std::vector<uint64_t> &Vect);

  /// DHDTGraph: Recursively execute hazard graph
  bool RecurseHazardGraph(DHDTNode *Node,
                          std::vector<uint64_t> &Vect);

  /// DHDTGraph: Examine the hazard vectors
  bool ExamineHazardResults(std::vector<std::vector<uint64_t>> HVect,
                            std::vector<unsigned> LineNo,
                            std::ofstream &Out);

  /// DHDTGraph: Accumulate the target link power
  double AccumLinkPower(DHDTLink *Link);

  /// DHDTGraph: Accumulate the target node power
  double AccumNodePower(DHDTNode *Node);

  /// DHDTGraph: Retrieve the width of the target operand
  unsigned GetValueWidth(Value *Op);

public:
  /// DHDTGraph: Constructor
  DHDTGraph(DHDTConfig &Config, bool Verbose);

  /// DHDTGraph: Destructor
  ~DHDTGraph();

  // DHDTGraph: Read the LLVM and CoreGen IR files
  bool ReadIR(std::string LLVMIR, std::string CoreGenIR);

  /// DHDTGraph: Build the graph
  bool BuildGraph();

  /// DHDTGraph: Build the DOT output file
  bool BuildDot(std::string DotFile);

  /// DHDTGraph: Initiate hazard analysis
  bool HazardAnalysis(std::string InstFile,
                      std::string OutFile);

  /// DHDTGraph: Initiate power analysis
  bool PowerAnalysis(std::string InstFile,
                     std::string PowerFile,
                     std::string OutFile);

};

#endif

// EOF
