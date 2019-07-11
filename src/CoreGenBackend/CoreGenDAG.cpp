//
// _CoreGenDAG_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenDAG.h"

CoreGenDAG::CoreGenDAG( CoreGenErrno *E )
  : Errno(E), DimSize(0),
  Level(0), Lower(nullptr), Higher(nullptr), isLower(false), AdjMat(nullptr){
}

CoreGenDAG::CoreGenDAG( CoreGenDAG *D,
                        unsigned **Mat,
                        unsigned Dim,
                        std::map<CoreGenNode *,unsigned> const &IMap,
                        CoreGenErrno *E )
  : Errno(E), DimSize(0),
  Level(D->GetLevel()+1), Lower(nullptr), Higher(D), isLower(false), AdjMat(nullptr) {
  if( Level == 1 ){
    LowerToL1(IMap);
  }else if( Level == 2 ){
    LowerToL2(IMap);
  }else if( Level == 3 ){
    LowerToL3(IMap);
  }else{
    // TODO: This really should throw an exception
    E->SetError(CGERR_ERROR,"No further lowering possible");
  }
}

CoreGenDAG::~CoreGenDAG(){
  // delete the lowered form
  if( isLower ){
    delete Lower;
  }

  // delete the adjacency matrix
  FreeMat();
}

bool CoreGenDAG::GenerateDAG(){
  //
  // For each node in our index map, retrieve each of its
  // children and insert the dependency into the adjacency
  // matrix IFF the mapping exists in the IdxMap
  //
  // We assume that the only nodes we care about are in the
  // IdxMap
  //
  for( auto const& it : IdxMap ){
    CoreGenNode *Node = it.first;
    for( unsigned i=0; i<Node->GetNumChild(); i++ ){
      CoreGenNode *CNode = Node->GetChild(i);
      auto Search = IdxMap.find(CNode);
      if( Search != IdxMap.end() ){
        // we found a valid index
        // insert it at AdjMat[Src][Child]
        AdjMat[it.second][Search->second] = 1;
      }
    }
  }

  return true;
}

bool CoreGenDAG::AllocMat(){
  if( AdjMat != nullptr ){
    return false;
  }
  AdjMat = new unsigned*[DimSize];
  if( AdjMat == nullptr ){
    return false;
  }
  for(unsigned i=0; i<DimSize; i++ ){
    AdjMat[i] = new unsigned[DimSize];
    if( AdjMat[i] == nullptr ){
      return false;
    }
  }

  for( unsigned i=0; i<DimSize; i++ ){
    for( unsigned j=0; j<DimSize; j++ ){
      AdjMat[i][j] = 0;
    }
  }

  return true;
}

bool CoreGenDAG::FreeMat(){
  if( AdjMat == nullptr ){
    return true;
  }

  for( unsigned i=0; i<DimSize; i++ ){
    delete [] AdjMat[i];
  }

  delete [] AdjMat;
  return true;
}

bool CoreGenDAG::BuildDAG( CoreGenNode *Top ){

  // Walk all the child nodes and build the index mapping.
  // Note that the TOP node is not included in the index map.
  // The TOP node is only a placeholder.
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    CoreGenNode *CNode = Top->GetChild(i);

    // check for collisions
    if( IdxMap.find(CNode) == IdxMap.end() ){
      IdxMap.insert(std::pair<CoreGenNode *,unsigned>(CNode,IdxMap.size()));
    }
  }

  // setup the dimension
  DimSize = IdxMap.size();
  if( DimSize == 0 ){
    Errno->SetError( CGERR_ERROR,
                     "No nodes found for the DAG" );
    return false;
  }
  // Allocate the matrix
  if( !AllocMat() ){
    Errno->SetError(CGERR_ERROR, "Could not allocate AdjMat" );
    return false;
  }

  return GenerateDAG();
}

bool CoreGenDAG::LowerAll(){
  if( Level < CG_MAX_DAG_LEVEL ){
    Lower = new CoreGenDAG(this,AdjMat,DimSize,IdxMap,Errno);
    if( Lower == nullptr ){
      Errno->SetError( CGERR_ERROR, "Could not lower DAG" );
      return false;
    }
    isLower = true;
    return Lower->LowerAll();
  }
  return true;
}

bool CoreGenDAG::LowerDAG(){
  if( Level < 3 ){
    Lower = new CoreGenDAG(this,AdjMat,DimSize,IdxMap,Errno);
    isLower = true;
    return true;
  }else{
    return false;
  }
}

bool CoreGenDAG::BuildDAGFromInst( CoreGenNode *Inst ){
  if( !Inst ){
    return false;
  }
  // walk all the child nodes
  for( unsigned i=0; i<Inst->GetNumChild(); i++ ){
    CoreGenNode *CNode = Inst->GetChild(i);

    // check for collisions
    if( IdxMap.find(CNode) == IdxMap.end() ){
      IdxMap.insert(std::pair<CoreGenNode *,unsigned>(CNode,IdxMap.size()));
    }
  }

  return true;
}

CoreGenDAG *CoreGenDAG::GetDAGFromLevel( unsigned L ){
  if( L > CG_MAX_DAG_LEVEL ){
    return nullptr;
  }else if( this->GetLevel() == L ){
    return this;
  }else if( this->GetLevel() < L ){
    return this->GetLower()->GetDAGFromLevel(L);
  }else{
    return this->GetHigher()->GetDAGFromLevel(L);
  }
}

bool CoreGenDAG::BuildDAGFromHW( CoreGenNode *HW ){
  if( !HW ){
    return false;
  }

  // walk all the child nodes
  for( unsigned i=0; i<HW->GetNumChild(); i++ ){
    CoreGenNode *CNode = HW->GetChild(i);

    // check for collisions
    if( CNode->GetType() == CGComm ){
      if( IdxMap.find(CNode) == IdxMap.end() ){
        IdxMap.insert(std::pair<CoreGenNode *,unsigned>(CNode,IdxMap.size()));
      }
    }
  }

  return true;
}

bool CoreGenDAG::BuildDAGFromPlugin( CoreGenNode *Plugin ){
  if( !Plugin ){
    return false;
  }

  // walk the standard node children
  for( unsigned i=0; i<Plugin->GetNumChild(); i++ ){
    CoreGenNode *CNode = Plugin->GetChild(i);

    // check for collisions
    if( IdxMap.find(CNode) == IdxMap.end() ){
      IdxMap.insert(std::pair<CoreGenNode *,unsigned>(CNode,IdxMap.size()));
    }

    // recursively add Plugin and Ext node children
    if( CNode->GetType() == CGPlugin ){
      if( !BuildDAGFromPlugin( CNode ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error in recursively populating plugins during L0-L1 lowering" );
        return false;
      }
    }else if( CNode->GetType() == CGExt ){
      if( !BuildDAGFromExt( CNode ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error in recursively populating extensions during L0-L1 lowering" );
        return false;
      }
    }
  }

  // walk all the private members behind the plugin implementation
  CoreGenPlugin *P = static_cast<CoreGenPlugin *>(Plugin);
  P->BuildDAG();
  CoreGenNode *Top = P->GetTop();
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    CoreGenNode *CNode = Top->GetChild(i);

    if( CNode->GetType() != CGComm ){
      if( IdxMap.find(CNode) == IdxMap.end() ){
        IdxMap.insert(std::pair<CoreGenNode *,unsigned>(CNode,IdxMap.size()));
      }
    }

    // recursively add Plugin and Ext node children
    if( CNode->GetType() == CGPlugin ){
      if( !BuildDAGFromPlugin( CNode ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error in recursively populating plugins during L0-L1 lowering" );
        return false;
      }
    }else if( CNode->GetType() == CGExt ){
      if( !BuildDAGFromExt( CNode ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error in recursively populating extensions during L0-L1 lowering" );
        return false;
      }
    }
  }

  return true;
}

bool CoreGenDAG::BuildDAGFromExt( CoreGenNode *Ext ){
  if( !Ext ){
    return false;
  }
  // walk all the child nodes
  // we also want to recurisvively add the Ext nodes
  for( unsigned i=0; i<Ext->GetNumChild(); i++ ){
    CoreGenNode *CNode = Ext->GetChild(i);

    // check for collisions
    if( CNode->GetType() != CGComm ){
      if( IdxMap.find(CNode) == IdxMap.end() ){
        IdxMap.insert(std::pair<CoreGenNode *,unsigned>(CNode,IdxMap.size()));
      }
    }

    // recursively add Ext node children
    if( CNode->GetType() == CGExt ){
      if( !BuildDAGFromExt( CNode ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error in recursively populating extensions during L0-L1 lowering" );
        return false;
      }
    }
  }

  return true;
}

bool CoreGenDAG::LowerToL1( std::map<CoreGenNode *,unsigned> const &IMap ){
  // Lowering from L0 to L1 implies that expand all the references from
  // CoreGenExt and CoreGenPlugin nodes to a form that includes all its child types

  //-- Step 1, Copy index map to local
  //   We use this method in case we lower multiple times
  IdxMap.insert(IMap.begin(), IMap.end());

  //-- Step 2, for each Ext node, collect all its children and index them
  for( auto const& it : IMap ){
    CoreGenNode *Node = it.first;
    if( Node->GetType() == CGExt ){
      // this is an Ext node, retrieve all the child nodes
      if( !BuildDAGFromExt( Node ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error populating extensions during L0-L1 lowering" );
        return false;
      }
    }else if( Node->GetType() == CGPlugin ){
      // this is a Plugin node, retrieve all the child nodes
      if( !BuildDAGFromPlugin( Node ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error populating plugins during L0-L1 lowering" );
        return false;
      }
    }
  }

  //-- Step 3, Allocate the lowered matrix
  DimSize = IdxMap.size();

  // Allocate matrix
  if( !AllocMat() ){
    Errno->SetError(CGERR_ERROR, "Could not allocate AdjMat" );
    return false;
  }

  //-- Step 4, Remap our Adjacency Matrix using the new data
  return GenerateDAG();
}

bool CoreGenDAG::LowerToL3( std::map<CoreGenNode *,unsigned> const &IMap ){
  // Lowering from L2 to L3 implies that we expand all the instruction
  // and register encodings for all nodes in order to detect any final collisions

  //-- Step 1, Copy index map to local
  //  We use this method in case we lower multiple times
  IdxMap.insert( IMap.begin(), IMap.end() );

  //-- Step 2, for each Inst or PseudoInst node, index them
  for(auto const& it : IMap ){
    CoreGenNode *Node = it.first;
    if( (Node->GetType() == CGInst) ||
        (Node->GetType() == CGPInst) ){
      // this is an inst or pinst node
      if( !BuildDAGFromInst( Node ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error populating encodings during L2-L3 lowering" );
        return false;
      }
    }
  }

  //-- Step 3, Allocate the lowered matrix
  DimSize = IdxMap.size();

  // Allocate matrix
  if( !AllocMat() ){
    Errno->SetError(CGERR_ERROR, "Could not allocate AdjMat" );
    return false;
  }

  //-- Step 4, Remap our Adjacency Matrix using the new data
  return GenerateDAG();
}

bool CoreGenDAG::LowerToL2( std::map<CoreGenNode *,unsigned> const &IMap ){
  // Lowering from L1 to L2 implies that we expand all the communication links
  // for all nodes.  Generally speaking, hardware nodes are the only nodes that
  // should have comm links

  //-- Step 1, Copy index map to local
  //  We use this method in case we lower multiple times
  IdxMap.insert( IMap.begin(), IMap.end() );

  //-- Step 2, index the new comm nodes
  for(auto const& it : IMap ){
    CoreGenNode *Node = it.first;
    switch( Node->GetType() ){
    case CGSoc:
    case CGCore:
    case CGReg:
    case CGRegC:
    case CGCache:
    case CGExt:
    case CGComm:
      if( !BuildDAGFromHW( Node ) ){
        Errno->SetError( CGERR_ERROR,
                         "Error populating encodings during L1-L2 lowering" );
        return false;
      }
      break;
    default:
      // do nothing
      break;
    }
  }

  //-- Step 3, Allocate the lowered matrix
  DimSize = IdxMap.size();

  // Allocate matrix
  if( !AllocMat() ){
    Errno->SetError(CGERR_ERROR, "Could not allocate AdjMat" );
    return false;
  }

  //-- Step 4, Remap our Adjacency Matrix using the new data
  return GenerateDAG();
}

CoreGenNode *CoreGenDAG::FindNodeByIndex( unsigned Idx ){
  for(auto const& it : IdxMap ){
    if( it.second == Idx ){
      return it.first;
    }
  }
  return NULL;
}

unsigned CoreGenDAG::FindIndexByNode( CoreGenNode *N ){
  for(auto const& it : IdxMap ){
    if( it.first->GetName() == N->GetName() ){
      return it.second;
    }
  }
  return 0;
}

bool CoreGenDAG::ExportDOT( std::string FileName ){
  // sanity check the filename
  if( FileName.length() == 0 ){
    return false;
  }

  // make sure we have nodes to output
  if( DimSize == 0 ){
    return false;
  }

  // open the file
  std::fstream fs;

  if( Level == 0 ){
    // open for writing
    fs.open( FileName, std::fstream::out );

    // write out the header
    fs << "#" << std::endl;
    fs << "# CoreGenBackend IR in DAG form" << std::endl;
    fs << "#" << std::endl;
    fs << "# Generate graph using \" dot  -Tpng thisfile > thisfile.png\"" << std::endl;
    fs << "#" << std::endl;
    fs << std::endl << std::endl;
    fs << "digraph {" << std::endl;
    fs << " rankdir=LR;" << std::endl;
  }else{
    // open for appending
    fs.open( FileName, std::fstream::out | std::fstream::app );
  }

  fs << "subgraph cluster_Level" << std::to_string(Level) << " {" << std::endl;
  fs << "label=\"CoreGen IR Level " << std::to_string(Level) << "\";" << std::endl;
  fs << "style=filled;" << std::endl;
  fs << "color=lightgrey;" << std::endl;
  fs << "node [style=filled,color=white];" << std::endl;

  // walk the DAG
  for( unsigned i=0; i<DimSize; i++ ){

    // print out the node at Index i
    CoreGenNode *Node = FindNodeByIndex(i);

    fs << "node" << std::to_string(Level) << i << ";" << std::endl;
    fs << "node" << std::to_string(Level)
      << i << " [label=\"" << Node->GetName() << "\"];" << std::endl;

    // output instruction field encodings
    if( Level == 3 ){
      if( Node->GetType() == CGEnc ){
        CoreGenEncoding *E = static_cast<CoreGenEncoding *>(Node);
        fs << "node" << std::to_string(Level) << i << "Enc;" << std::endl;
        fs << "node" << std::to_string(Level) << i << "Enc [label=\"Enc="
          << E->GetEncoding() << "\"];" << std::endl;
        fs << "node" << std::to_string(Level) << i << "-> node"
          << std::to_string(Level) << i << "Enc;" << std::endl;
      }

      // output register index encodings
      if( Node->GetType() == CGReg ){
        CoreGenReg *E = static_cast<CoreGenReg *>(Node);
        fs << "node" << std::to_string(Level) << i << "Idx;" << std::endl;
        fs << "node" << std::to_string(Level) << i << "Idx [label=\"Idx="
          << E->GetIndex() << "\"];" << std::endl;
        fs << "node" << std::to_string(Level) << i << "-> node"
          << std::to_string(Level) << i << "Idx;" << std::endl;
      }
    }

    // output each row's children
    for( unsigned j=0; j<DimSize; j++ ){
      if( AdjMat[i][j] == 1 ){
        // output a dependence
        fs << "node" << std::to_string(Level) << i
          << "-> node" << std::to_string(Level) << j << ";" << std::endl;
      }
    } // end for-j
  }

  fs << "}" << std::endl; // end subgraph

  if( !isLower ){
    fs << "}" << std::endl; // end graph
  }

  // close the file
  fs.close();

  if( isLower ){
    if( !Lower->ExportDOT( FileName ) ){
      Errno->SetError( CGERR_ERROR,
                       "Error writing lowered DAG to DOT output at " + FileName );
      return false;
    }
  }

  return true;
}

// EOF
