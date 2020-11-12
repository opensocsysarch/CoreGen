//
// _PoarAccum_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarAccum
 *
 * \ingroup Poar
 *
 * \brief Power/Area accumulation base class
 *
 */

#ifndef _POARACCUM_H_
#define _POARACCUM_H_

//-- C++ headers
#include <string>
#include <vector>

//-- CoreGen headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class PoarAccum{
public:
  /// PoarAccum: default constructor
  PoarAccum(CoreGenNode *T, std::string N)
    : Top(T), Width(0x00ull), Name(N), HasRun(false) { }

  /// PoarAccum: default destructor
  ~PoarAccum();

  /// PoarAccum: Retrieve the accumulated number of nodes
  unsigned GetNumNodes() { return NodeVect.size(); }

  /// PoarAccum: Retrieve the accumulated node widths (nodes x width)
  uint64_t GetWidth() { return Width; }

  /// PoarAccum: Retrieve the name of the accumulator pass
  std::string GetName() { return Name; }

  /// PoarAccum: Node accumulation function
  virtual bool Accum() = 0;

protected:
  /// PoarAccum: accumulated a node
  bool AddNode(CoreGenNode *N) { NodeVect.push_back(N); return true;}

  /// PoarAccum: accumulate a bit width
  bool AddWidth(uint64_t W) { Width += W; return true; }

  /// PoarAccum: does the node exist in the node vector?
  bool Exists(CoreGenNode *N){
    auto it = std::find(NodeVect.begin(),NodeVect.end(),N);
    if( it != NodeVect.end() )
      return true;
    return false;
  }

  /// PoarAccum: recursively visit and accumulate nodes of the target type
  bool AccumNodeVisitor(std::vector<CoreGenNode *> &Nodes,
                        CGNodeType Type,
                        CoreGenNode *T){
    for( unsigned i=0; i<T->GetNumChild(); i++ ){
      if( T->GetChild(i)->GetType() == Type )
        Nodes.push_back(T->GetChild(i));
      AccumNodeVisitor(Nodes,Type,T->GetChild(i));
    }
    return true;
  }

  /// PoarAccum: recusively visit and accumulate nodes of the target type from Top down
  bool AccumNodes(std::vector<CoreGenNode *> &Nodes,
                  CGNodeType Type ){
    return AccumNodeVisitor(Nodes,Type,Top);
  }

  /// PoarAccum: flag the pass as having being run already
  void FlagRun() { HasRun = true; }

private:
  CoreGenNode *Top;     ///< PoarAccum: Top-level node to begin accumulation discovery
  uint64_t Width;       ///< PoarAccum: accumulated width of all nodes
  std::string Name;     ///< PoarAccum: name of the accumulator
  bool HasRun;          ///< PoarAccum: flag to determine whether the pass has run

  std::vector<CoreGenNode *> NodeVect;  ///< PoarAccum: vector of discovered node names

};

#endif

// EOF
