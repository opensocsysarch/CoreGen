
//
// _DPathCG_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class DPathCG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief DPathCG generates the datapath implementation capable of executing the instructions 
 * generated by the CoreGen uCode compiler
 *
 */


#ifndef _DPATHCG_H_
#define _DPATHCG_H_

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

class DPathCG : public CoreGenNodeCodegen {
private:

  /// Generate the shared bus
  bool          GenSharedBus(std::ofstream &O);

  /// Generate the instruction dispatch
  bool          GenInstDispatch(std::ofstream &O ) { return false;};

  /// Generate the ALU
  bool          GenALU(std::ofstream &O ){return false; };


public:

  /// Default constructor
  DPathCG(CoreGenNode *N,
          CoreGenProj *P,
          std::string Package,
          std::string Path,
          bool Common,
          CoreGenErrno *E );

  /// Execute the codegen
  bool    Execute( );

};

#endif

