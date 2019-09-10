//
// _DanglingRegionPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/DanglingRegionPass.h"

DanglingRegionPass::DanglingRegionPass(std::ostream *O,
                                       CoreGenDAG *D,
                                       CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,3,"DanglingRegionPass",
                "Identifies any potential dangling regions or modules",
                false,O,D,E) {
}

DanglingRegionPass::~DanglingRegionPass(){
}

// performs a depth first search of the graph
// we consider it undirected for this DFS
// as we're only considering general connectivity
// https://www.geeksforgeeks.org/connected-components-in-an-undirected-graph/
void DanglingRegionPass::DFS( CoreGenDAG *D3,
                              unsigned DimSize,
                              unsigned v,
                              bool visited[] ){
  visited[v] = true;

  for( unsigned j=0; j<DimSize; j++ ){
    if( D3->AdjMat[v][j] == 1 ){
      if( !visited[j] ){
        DFS(D3,DimSize,j,visited);
      }
    }
    //if( D3->AdjMat[j][v] == 1 ){
    if( D3->TransAdjMat[v][j] == 1 ){
      if( !visited[j] ){
        DFS(D3,DimSize,j,visited);
      }
    }
  }
}

bool DanglingRegionPass::Execute(){

  // get the target DAG level
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  unsigned D = D3->GetDimSize();

  // init the visited vector
  bool *visited = new bool[D];
  for( unsigned i=0; i<D; i++ ){
    visited[i] = false;
  }

  unsigned ConnectComp = 0;
  bool rtn = true;

  for( unsigned i=0; i<D; i++ ){
    if( visited[i] == false ){
      // perform a depth first search
      DFS(D3,D,i,visited);

      // nudge the connected components
      ConnectComp++;
    }
  }

  if( ConnectComp > 1 ){
    rtn = false;
    WriteMsg( "Identified dangling component regions: "
              + std::to_string(ConnectComp) + " Regions" );
  }

  delete[] visited;

  return rtn;
}
