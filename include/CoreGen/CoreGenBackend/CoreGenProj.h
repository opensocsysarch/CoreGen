//
// _CoreGenProj_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenProj
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Project Handler
 *
 */

#ifndef _COREGENPROJ_H_
#define _COREGENPROJ_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

typedef enum{
  CGSocProj = 0,      ///< CGProjType: SoC Project
  CGModProj = 1,      ///< CGProjType: Module Project
  CGExtProj = 10,     ///< CGProjType: Extension Project
  CGNULLProj = 11     ///< CGProjType: Unknown Project
}CGProjType;          ///< CoreGenProj: Project Types

class CoreGenProj{
private:
  CoreGenErrno *Errno;    ///< CoreGenProj: Errno structure
  std::string ProjName;   ///< CoreGenProj: Project name
  std::string ProjRoot;   ///< CoreGenProj: Project root
  CGProjType Type;        ///< CoreGenProj: Project type
  unsigned ChiselMajor;   ///< CoreGenProj: Chisel Major Version
  unsigned ChiselMinor;   ///< CoreGenProj: Chisel Minor Version
public:
  /// Default Constructor
  CoreGenProj( CoreGenErrno *E );

  /// Overloaded Constructor
  CoreGenProj( CoreGenErrno *E,
               std::string N,
               std::string R );

  /// Overloaded Constructor
  CoreGenProj( CoreGenErrno *E,
               std::string N,
               std::string R,
               CGProjType T );

  /// Default Destructor
  ~CoreGenProj();

  /// Set the chisel version
  bool SetChiselVersion( unsigned Major, unsigned Minor );

  /// Set the project name
  bool SetProjName(std::string N);

  /// Set the project root
  bool SetProjRoot(std::string R);

  /// Retrieve the chisel version
  bool GetChiselVersion( unsigned *Major, unsigned *Minor );

  /// Retrieve the project name
  std::string GetProjName();

  /// Retrieve the project root
  std::string GetProjRoot();

  /// Set the project type
  bool SetProjType( CGProjType T );

  /// Retrieve the project type
  CGProjType GetProjType();
};

#endif

// EOF
