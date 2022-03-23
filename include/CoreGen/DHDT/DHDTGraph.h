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

#include <iostream>
#include <string>
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

class DHDTGraph{
private:
  // private variables
  SMDiagnostic Err;               ///< DHDTGraph: LLVM diagnostic errors
  std::unique_ptr<Module> Mod;    ///< DHDTGraph: LLVM module pointer
  LLVMContext Context;            ///< DHDTGraph: LLVM Context

public:
  /// DHDTGraph: Constructor
  DHDTGraph();

  /// DHDTGraph: Destructor
  ~DHDTGraph();

  /// DHDTGraph: Read the LLVM IR into graph form
  bool ReadLLVMIRGraph(std::string IR);
};

#endif

// EOF
