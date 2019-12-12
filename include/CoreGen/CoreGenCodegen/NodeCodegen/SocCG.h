//
// _SocCG_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SocCG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenNodeCodegen derived class for SoC's
 *
 */

#ifndef _SOCCG_H_
#define _SOCCG_H_

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

class SocCG : public CoreGenNodeCodegen {
private:
  /// Write the SoC Chisel output
  bool WriteSoc(std::ofstream &O);

public:
  /// Default constructor
  SocCG(CoreGenNode *N,
         CoreGenProj *P,
         std::string Package,
         std::string Path,
         bool Common,
         CoreGenErrno *E );

  /// Default destructor
  ~SocCG();

  /// Execute the codegen
  virtual bool Execute();
};

#endif

// EOF
