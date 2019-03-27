//
// _SpadCG_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SpadCG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenNodeCodegen derived class for scratchpads
 *
 */

#ifndef _SPADCG_H_
#define _SPADCG_H_

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

class SpadCG : public CoreGenNodeCodegen {
private:
  /// Write the scratchpad block to the file
  bool WriteSpad(std::ofstream &O);

public:
  /// Default constructor
  SpadCG(CoreGenNode *N,
         CoreGenProj *P,
         std::string Package,
         std::string Path,
         bool Common,
         CoreGenErrno *E );

  /// Default destructor
  ~SpadCG();

  /// Execute the codegen
  virtual bool Execute();
};

#endif

// EOF
