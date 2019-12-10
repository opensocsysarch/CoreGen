//
// _CoreGenNodeCodegen_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenNodeCodegen
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenCodegen base class for node code generation facilities
 *
 */

#ifndef _COREGENNODECODEGEN_H_
#define _COREGENNODECODEGEN_H_

// Standard Headers
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenNodes.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

class CoreGenNodeCodegen{
protected:
  CoreGenNode *Node;                ///< Raw node object
  CoreGenProj *Proj;                ///< CoreGen Project Info
  std::string Package;              ///< Chisel package name for this target
  std::string Path;                 ///< Target output file
  bool Common;                      ///< Is this a common package?
  CoreGenErrno *Errno;              ///< CoreGen Errno Structure

public:
  /// Default constructor
  CoreGenNodeCodegen( CoreGenNode *N,
                      CoreGenProj *P,
                      std::string Pac,
                      std::string Pa,
                      bool Comm,
                      CoreGenErrno *E ) : Node(N), Proj(P), Package(Pac),
                                          Path(Pa), Common(Comm), Errno(E){}

  /// Default destructor
  virtual ~CoreGenNodeCodegen() {}

  /// Executes the node-specific code generator
  virtual bool Execute() { return false; }

  /// Writes the top-level Chisel package info
  bool WritePackageInfo(std::ofstream &O);

  /// Writes the standard Chisel imports out to the target output stream
  bool WriteChiselImports(std::ofstream &O);

  /// Writes the standard Chisel footer
  bool WriteStdFooter(std::ofstream &O);
};

#endif

// EOF
