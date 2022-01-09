//
// _SCVLIWPipeBuilder_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCVLIWPipeBuilder
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Constructs the arithmetic portions of the Chisel pipeline
 *
 */

#ifndef _STONECUTTER_SCPASS_SCVLIWPIPEBUILDER_H_
#define _STONECUTTER_SCPASS_SCVLIWPIPEBUILDER_H_

#include <map>
#include <vector>
#include <tuple>
#include "CoreGen/StoneCutter/SCPass.h"
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

class VLIWSig{
private:
  std::string Name;                 ///< VLIWSig: name of the signal
  unsigned Width;                   ///< VLIWSig: width (in bits) of the signal
  SigType Type;                     ///< VLIWSig: type of the signal

public:
  /// VLIWSig: default constructor
  VLIWSig(std::string N,
          unsigned W,
          SigType T )
    : Name(N), Width(W), Type(T) {}

  /// VLIWSig: default destructor
  ~VLIWSig();

  /// VLIWSig: retrieve the name of the signal
  std::string GetName() { return Name; }

  /// VLIWSig: retrieve the width of the signal
  unsigned GetWidth() { return Width; }

  /// VLIWSig: retrieve the type of the signal
  SigType GetType() { return Type; }
};

class VLIWEdge{
private:
  std::string Src;                  ///< VLIWEdge: source node name
  std::string Dest;                 ///< VLIWEdge: destination node name
  std::vector<VLIWSig *> Signals;   ///< VLIWEdge: vector of signals on the edge

public:
  /// VLIWEdge: default constructor
  VLIWEdge(std::string S,
           std::string D)
    : Src(S), Dest(D) { }

  /// VLIWEdge: default destructor
  ~VLIWEdge() {Signals.clear();}

  /// VLIWEdge: insert a new signal into the edge
  void InsertSignal(VLIWSig *S){Signals.push_back(S);}

  /// VLIWEdge: retrieve the number of signals on the edge
  unsigned GetNumSignals(){return Signals.size();}

  /// VLIWEdge: retrieve a specific signal from the edge
  VLIWSig *GetSignal(unsigned J){
    if( J >((Signals.size())-1) )
      return nullptr;
    return Signals[J];
  }

  /// VLIWEdge: retrieve the source node name
  std::string GetSrc() { return Src; }

  /// VLIWEdge: retrieve the destination node name
  std::string GetDest() { return Dest; }
};

class VLIWNode{
private:
  std::string Name;                 ///< VLIWNode: name of the target node
  unsigned Stage;                   ///< VLIWNode: numeric identifier of the pipeline stage
  std::vector<VLIWEdge *> Edges;    ///< VLIWNode: vector of edge objects

public:
  /// VLIWNode: default constructor
  VLIWNode(std::string N,
           unsigned S)
    : Name(N), Stage(S) {}

  /// VLIWNode: default destructor
  ~VLIWNode() {Edges.clear();}

  /// VLIWNode: insert an edge into the node
  void InsertEdge(VLIWEdge *E){Edges.push_back(E);}

  /// VLIWNode: retrieve the number of edges
  unsigned GetNumEdges(){return Edges.size();}

  /// VLIWNode: retrieve a specific edge
  VLIWEdge *GetEdge(unsigned J){
    if( J > ((Edges.size())-1) )
      return nullptr;
    return Edges[J];
  }

  /// VLIWNode: retrieve the ame of the node
  std::string GetName() { return Name; }

  /// VLIWNode: retrieve the respective pipeline stage
  unsigned GetStage() { return Stage; }
};

class VLIWGraph{
private:
  std::vector<VLIWNode *> Nodes;    ///< VLIWGraph: vector of node objects

public:
  /// VLIWGraph: default constructor
  VLIWGraph() { }

  /// VLIWGraph: default destructor
  ~VLIWGraph() { Nodes.clear(); }

  /// VLIWGraph: Insert a new node into the graph
  void InsertNode( VLIWNode *N) { Nodes.push_back(N); }

  /// VLIWGraph: retrieve the number of nodes in the graph
  unsigned GetNumNodes() { return Nodes.size(); }

  /// VLIWGraph: retrieve a specific node by its index
  VLIWNode *GetNode(unsigned J){
    if( J > ((Nodes.size())-1) )
      return nullptr;
    return Nodes[J];
  }

  /// VLIWGraph: retreive a specific node by its name and stage index
  VLIWNode *GetNodeByName(std::string Func,
                          unsigned Stage){
    VLIWNode *N = nullptr;
    for( unsigned i=0; i<Nodes.size(); i++ ){
      if( (Nodes[i]->GetName() == Func) &&
          (Nodes[i]->GetStage() == Stage) ){
        N = Nodes[i];
      }
    }
    return N;
  }
};

class SCVLIWPipeBuilder : public SCPass {
private:
  CoreGenSigMap *SigMap;              ///< SCVLIWPipeBuilder: Signal map object
  VLIWGraph *Graph;                   ///< SCVLIWPipeBuilder: I/O Graph object

  std::vector<std::pair<std::string,bool (SCVLIWPipeBuilder::*)()>> Enabled;   ///< SCVLIWPipeBuilder: Enabled sub-passes

  std::vector<std::pair<std::string,unsigned>> VLIWStages;  ///< SCVLIWPipeBuilder: VLIW Stages

  /// SCVLIWPipeBuilder: Enable sub-passes
  bool EnableSubPasses();

  /// SCVLIWPipeBuilder: Execute the various optimization phases
  bool Optimize();

  /// SCVLIWPipeBuilder: Write the signal map back
  bool WriteSigMap();

  /// SCVLIWPipeBuilder: Derive the pipeline stages that are VLIW
  bool DeriveVLIWStages();

  // ----------------------------------------------------------
  // Sub Passes
  // ----------------------------------------------------------

  /// SCVLIWPipeBuilder SubPass: Determines whether the pipeline stages are monotonically increasing
  bool DiscoverPipeSlots();

  /// SCVLIWPipeBuilder SubPass: Peforms wireup of all input/output functions of each stage
  bool WireUpStages();

  /// SCVLIWPipeBuilder SubSubPass: Wires up all the I/Os between intrinsics and arguments
  bool WireIO(VLIWGraph *Graph);

  /// SCVLIWPipeBuilder SubSubPass: Creates the correct edges and signals for the Src/Dest pairs
  void InsertSignalEdge(VLIWGraph *Graph,
                        std::string SrcFunc,
                        std::string DestFunc,
                        unsigned SrcStage,
                        unsigned DestStage,
                        std::string Arg,
                        std::string Type,
                        unsigned Width);

public:
  /// Default cosntructor
  SCVLIWPipeBuilder(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCVLIWPipeBuilder();

  /// Set the signal map object
  bool SetSignalMap(CoreGenSigMap *CSM){ SigMap = CSM; return true; }

  /// Execute the codegen
  virtual bool Execute() override;
};

#endif


