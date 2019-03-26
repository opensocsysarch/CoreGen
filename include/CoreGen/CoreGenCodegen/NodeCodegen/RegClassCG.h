//
// _RegClassCG_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class RegClassCG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenNodeCodegen derived class for register classes
 *
 */

#ifndef _REGCLASSCG_H_
#define _REGCLASSCG_H_

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

class RegClassCG : public CoreGenNodeCodegen{
private:
  /// Writes the register class block to the output stream
  bool WriteRegClass( std::ofstream &O );

public:
  /// Default constructor
  RegClassCG(CoreGenNode *N,
             CoreGenProj *P,
             std::string Package,
             std::string Path,
             bool Common,
             CoreGenErrno *E );

  /// Default destructor
  ~RegClassCG();

  /// Execute the codegen
  virtual bool Execute();
};

#endif

// EOF
