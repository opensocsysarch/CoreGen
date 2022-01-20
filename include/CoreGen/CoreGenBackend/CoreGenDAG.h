//
// _CoreGenDAG_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenDAG
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen DAG Handler
 *
 */

#ifndef _COREGENDAG_H_
#define _COREGENDAG_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"  // for DOT printing
#include "CoreGen/CoreGenBackend/CoreGenReg.h"       // for DOT printing
#include "CoreGen/CoreGenBackend/CoreGenPlugin.h"    // for Plugin processing
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"      // for ATTRIBUTE_NO_SANITIZE_ADDRESS

#define CG_MAX_DAG_LEVEL 3

class CoreGenDAG{
private:
  CoreGenErrno *Errno;                      ///< CoreGenDAG: Error handler

  std::map<CoreGenNode *,unsigned> IdxMap;  ///< CoreGenDAG: Index mapping for each node

  unsigned DimSize;                         ///< Size of one dimension of matrix

  // Lower and Higher DAG Forms
  unsigned Level;                           ///< Lowering Level, Smaller = Less Info
  CoreGenDAG *Lower;                        ///< Lower DAG form
  CoreGenDAG *Higher;                       ///< Higher DAG form

  bool isLower;                             ///< Determines if there is a Lower DAG

  /// Allocate a matrix
  bool AllocMat();

  /// Free a matrix
  bool FreeMat();

  /// Generates the DAG based upon the contents of the index map
  bool GenerateDAG();

  // Lowering Functions
  /// Lowers from L0 to L1
  bool LowerToL1( std::map<CoreGenNode *,unsigned> const &IMap );

  /// Lowers from L1 to L2
  bool LowerToL2( std::map<CoreGenNode *,unsigned> const &IMap );

  /// Lowers from L2 to L3
  bool LowerToL3( std::map<CoreGenNode *,unsigned> const &IMap );

  /// Builds the DAG from an existing L0 Ext node
  bool BuildDAGFromExt( CoreGenNode *Ext );

  /// Builds the DAG from an existing L0 Plugin node
  bool BuildDAGFromPlugin( CoreGenNode *Plugin );

  /// Builds the DAG from an existing L0/L1 Inst node (expands encodings)
  bool BuildDAGFromInst( CoreGenNode *Ext );

  /// Builds the DAG from an existing L2 node (expands comm channels)
  bool BuildDAGFromHW( CoreGenNode *HW );

  /// Passes as friend classes
  // -- Analysis Passes
  friend class DanglingNodePass;
  friend class DanglingRegionPass;
  friend class ICacheCheckerPass;
  friend class CacheLevelPass;
  friend class EncodingCollisionPass;
  friend class RegIdxPass;
  friend class RegFieldPass;
  friend class RegSafetyPass;
  friend class SpadSafetyPass;
  friend class RegClassSafetyPass;
  friend class L1SharedPass;
  friend class MultSoCPass;
  friend class MandatoryFieldPass;
  friend class EncodingGapPass;
  friend class CoreSafetyPass;
  friend class SafeDeletePass;
  friend class CommSafetyPass;
  friend class PInstSafetyPass;
  friend class CommSafetyPass;
  friend class ASPSolverPass;

  // -- Data Passes
  friend class StatsPass;
  friend class InstTable;
  friend class SpecDoc;

  // -- Optimization passes
  friend class RegClassOpt;
  friend class CacheLevelOpt;

protected:
  unsigned **AdjMat;                        ///< Adjacency matrix for the DAG
  unsigned **TransAdjMat;                   ///< Transposed adjacency matrix for the DAG

  /// Find the node pointer by the mapped index
  CoreGenNode *FindNodeByIndex( unsigned I );

  /// Find the index by the node pointer
  unsigned FindIndexByNode( CoreGenNode *N );

public:

  /// Default constructor
  CoreGenDAG(CoreGenErrno *E);

  /// Overloaded constructor
  CoreGenDAG(CoreGenDAG *,unsigned **,unsigned,
             std::map<CoreGenNode *,unsigned> const &,CoreGenErrno *);

  /// Default destructor
  ~CoreGenDAG();

  /// Retrieve an object pointer from the target level
  CoreGenDAG *GetDAGFromLevel( unsigned Level );

  /// Retrieve an object pointer to the lower DAG form
  CoreGenDAG *GetLower() { return Lower; }

  /// Retrieve an object pointer to the higher DAG form
  CoreGenDAG *GetHigher() { return Higher; }

  /// Retrieve the lowered level of this DAG
  unsigned GetLevel() { return Level; }

  /// Retrieve the dimension size
  unsigned GetDimSize() { return DimSize; }

  /// Construct the high level DAG representation
  bool BuildDAG( CoreGenNode *Top );

  /// Lower the DAG to the next logical level
  bool LowerDAG();

  /// Lower all the DAG levels automatically
  bool LowerAll();

  /// Export the DAG's in DOT format
  bool ExportDOT( std::string FileName );
};

#endif

// EOF
