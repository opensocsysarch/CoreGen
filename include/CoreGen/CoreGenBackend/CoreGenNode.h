//
// _CoreGenNode_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenNode
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Base Node Handler
 *
 */


#ifndef _COREGENNODE_H_
#define _COREGENNODE_H_

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

typedef enum{
  CGSoc   = 0,                          ///< CGNodeType: SoC Node
  CGCore  = 1,                          ///< CGNodeType: Core Node
  CGInstF = 2,                          ///< CGNodeType: Instruction Format Node
  CGInst  = 3,                          ///< CGNodeType: Instruction Node
  CGPInst = 4,                          ///< CGNodeType: PseudoInstruction Node
  CGRegC  = 5,                          ///< CGNodeType: Register Class Node
  CGReg   = 6,                          ///< CGNodeType: Register Node
  CGISA   = 7,                          ///< CGNodeType: ISA Node
  CGCache = 8,                          ///< CGNodeType: Cache Node
  CGEnc   = 9,                          ///< CGNodeType: Encoding
  CGExt   = 10,                         ///< CGNodeType: Extension Node
  CGComm  = 11,                         ///< CGNodeType: Communication Channel
  CGSpad  = 12,                         ///< CGNodeType: Scratchpad
  CGMCtrl = 13,                         ///< CGNodeType: Memory Controller
  CGVTP   = 14,                         ///< CGNodeType: Virtual to Physical Node
  CGPlugin= 30,                         ///< CGNodeType: Plugin
  CGTop   = 100                         ///< CGNodeType: Top-Level Node
}CGNodeType;                            ///< CoreGenNode: CoreGen Node Types

typedef enum{
  RTLUnk     = 0,                       ///< CGRTLType: Unknown RTL Type
  RTLChisel  = 1,                       ///< CGRTLType: Chisel RTL
  RTLVerilog = 2                        ///< CGRTLType: Verilog RTL
}CGRTLType;                             ///< CoreGenNode: Inline RTL Type

typedef enum{
  AttrSharedReg = 0,                    ///< CGAttr: Shared register file attribute
  AttrPrivReg = 1,                      ///< CGAttr: Private register file attribute
  AttrTUSReg = 2,                       ///< CGAttr: Thread unit shared registers
  AttrISAReg = 3,                       ///< CGAttr: Multi-ISA shared registers
  AttrSharedCache = 4                   ///< CGAttr: Shared cache level attribute
}CGAttr;                                ///< CoreGenNode: CoreGen Node Attributes

class CoreGenNode{
private:
  std::string Name;                     ///< CoreGenNode: Name of the CoreGen node
  std::string RTL;                      ///< CoreGenNode: User-directed, inline RTL
  std::string RTLFile;                  ///< CoreGenNode: User-directed external RTL file
  CGNodeType Type;                      ///< CoreGenNode: Type of CoreGen node
  CGRTLType RTLType;                    ///< CoreGenNode: Type of RTL
  std::vector<CGAttr> Attrs;            ///< CoreGenNode: Node attributes
  std::vector<CoreGenNode *> CNodes;    ///< CoreGenNode: Child DAG nodes
  std::string ASP;                      ///< CoreGenNode: ASP representation of the node
  std::string Notes;                    ///< CoreGenNode: Notes associated with the target node

  CoreGenNode *PluginNode;              ///< CoreGenNode: Overridden Plugin Node

protected:
  CoreGenErrno *Errno;                  ///< CoreGenNode: Errno handler

public:

  /// Default Constructor
  CoreGenNode();

  /// Overloaded Constructor
  CoreGenNode(CGNodeType T, CoreGenErrno *E);

  /// Overloaded Constructor
  CoreGenNode(CGNodeType T, std::string N, CoreGenErrno *E);

  /// Default Destructor
  ~CoreGenNode();

  /// Retrieve the CoreGen node type
  CGNodeType GetType();

  /// Insert an attribute for this node
  bool SetAttr(CGAttr A);

  /// Determine if the target attribute is set
  bool HasAttr(CGAttr A);

  /// Set the notes for the target node
  bool SetNotes(std::string S);

  /// Set the CoreGen node type
  bool SetType(CGNodeType T);

  /// Set the RTL Type
  bool SetRTLType(CGRTLType T);

  /// Set the CoreGen errno handler
  bool SetErrno(CoreGenErrno *E);

  /// Retrieve the node name
  std::string GetName();

  /// Set the node name
  bool SetName(std::string N);

  /// Determines whether the requested node is a duplicate of an existing child
  bool IsDuplicateNode( CoreGenNode *N );

  /// Retrieve the number of child nodes
  unsigned GetNumChild();

  /// Retrieve a pointer to the target child node
  CoreGenNode *GetChild(unsigned I);

  /// Insert a child node
  bool InsertChild( CoreGenNode *N );

  /// Delete all child nodes equivalent to input
  bool DeleteChild( CoreGenNode *N );

  /// Purge all the child nodes from node
  bool PurgeChildren();

  /// Insert a plugin node
  bool InsertPlugin( CoreGenNode *N );

  /// Delete a plugin node
  bool DeletePlugin();

  /// Determine if there is a plugin node to override this node
  bool IsPlugin();

  /// Retrieves the target plugin node
  CoreGenNode *GetPlugin(){ return PluginNode; }

  /// Set inline RTL
  bool SetRTL( std::string RTL );

  /// Determine whether there is inline RTL
  bool IsRTL();

  /// Retrieve the type of RTL
  CGRTLType GetRTLType();

  /// Retrieve inline RTL
  std::string GetRTL();

  /// Set external RTL file
  bool SetRTLFile( std::string RTLFile );

  /// Determine whether there is an external RTL file defined
  bool IsRTLFile();

  /// Retrieve the external RTL File
  std::string GetRTLFile();

  /// Is the current node overridden with a plugin?
  bool IsOverridden();

  /// Retrieve the notes for the target node
  std::string GetNotes();

  /// Determines if there is a set of valid notes for the target node
  bool HasNotes();

  /// Retrieve the current node plugin pointer
  CoreGenNode *GetOverriddenNode();

  /// Remove the overridden node pointer but do not delete the overridden node
  bool RemoveOverriddenNode();

  /// Set the overridden plugin node
  bool SetOverriddenNode( CoreGenNode *Node );

  /// Append to the ASP String
  void AppendASP( std::string ASPString) { ASP += ASPString; }

  /// Retrieve the ASP for the target node
  std::string GetASP() { return ASP; }
};

#endif

// EOF
