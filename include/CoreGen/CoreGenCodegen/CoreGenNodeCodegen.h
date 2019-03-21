//
// _CoreGenNodeCodegen_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

class CoreGenNodeCodegen{
protected:
  CoreGenNode *Node;                ///< Raw node object
  CoreGenProj *Proj;                ///< CoreGen Project Info
  std::string Package;              ///< Chisel package name for this target
  std::string Path;                 ///< Target output file
  CoreGenErrno *Errno;              ///< CoreGen Errno Structure

public:
  /// Default constructor
  CoreGenNodeCodegen( CoreGenNode *N,
                      CoreGenProj *P,
                      std::string Pac,
                      std::string Pa,
                      CoreGenErrno *E ) : Node(N), Proj(P), Package(Pac),
                                          Path(Pa), Errno(E){}

  /// Default destructor
  virtual ~CoreGenNodeCodegen() {}

  /// Executes the node-specific code generator
  virtual bool Execute() { return false; }

  /// Writes the top-level Chisel package info
  bool WritePackageInfo(std::ofstream &O){
    if( !O.is_open() )
      return false;
    O << "// -- Chisel package info" << std::endl;
    O << "package " << Package << std::endl;
    return true;
  }

  /// Writes the standard Chisel imports out to the target output stream
  bool WriteStdChiselImport(std::ofstream &O){
    if( !O.is_open() )
      return false;
    O << "// -- Import standard Chisel and Scala packages" << std::endl;
    O << "import Chisel._" << std::endl;
    O << "import Chisel.ImplicitConversions._" << std::endl;
    O << "import scala.collection.immutable.ListMap" << std::endl;
    O << "import scala.collection.mutable.ArrayBuffer" << std::endl;
    return true;
  }
};

#endif

// EOF
