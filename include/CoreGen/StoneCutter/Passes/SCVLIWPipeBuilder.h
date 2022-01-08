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
  std::string Name;
  unsigned Width;
  SigType Type;

public:
  VLIWSig(std::string N,
          unsigned W,
          SigType T )
    : Name(N), Width(W), Type(T) {}
  ~VLIWSig();
};

class VLIWEdge{
private:
  std::string Src;
  std::string Dest;
  std::vector<VLIWSig *> Signals;

public:
  VLIWEdge(std::string S,
           std::string D)
    : Src(S), Dest(D) { }
  ~VLIWEdge() {Signals.clear();}
  void InsertSignal(VLIWSig *S){Signals.push_back(S);}
  unsigned GetNumSignals(){return Signals.size();}
  VLIWSig *GetSignal(unsigned J){
    if( J >((Signals.size())-1) )
      return nullptr;
    return Signals[J];
  }
  std::string GetSrc() { return Src; }
  std::string GetDest() { return Dest; }
};

class VLIWNode{
private:
  std::string Name;
  unsigned Stage;
  std::vector<VLIWEdge *> Edges;

public:
  VLIWNode(std::string N,
           unsigned S)
    : Name(N), Stage(S) {}
  ~VLIWNode() {Edges.clear();}
  void InsertEdge(VLIWEdge *E){Edges.push_back(E);}
  unsigned GetNumEdges(){return Edges.size();}
  VLIWEdge *GetEdge(unsigned J){
    if( J > ((Edges.size())-1) )
      return nullptr;
    return Edges[J];
  }
  std::string GetName() { return Name; }
  unsigned GetStage() { return Stage; }
};

class VLIWGraph{
private:
  std::vector<VLIWNode *> Nodes;

public:
  VLIWGraph() { }
  ~VLIWGraph() { Nodes.clear(); }
  void InsertNode( VLIWNode *N) { Nodes.push_back(N); }
  unsigned GetNumNodes() { return Nodes.size(); }
  VLIWNode *GetNode(unsigned J){
    if( J > ((Nodes.size())-1) )
      return nullptr;
    return Nodes[J];
  }
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


