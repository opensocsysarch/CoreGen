//
// _CacheCG_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CacheCG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenNodeCodegen derived class for SoC's
 *
 */

#ifndef _CACHECG_H_
#define _CACHECG_H_

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

class CacheCG : public CoreGenNodeCodegen {
private:

public:
  /// Default constructor
  CacheCG(CoreGenNode *N,
         CoreGenProj *P,
         std::string Package,
         std::string Path,
         bool Common,
         CoreGenErrno *E );

  /// Default destructor
  ~CacheCG();

  /// Execute the plugin codegen
  virtual bool ExecutePlugin();

  /// Execute the codegen
  virtual bool Execute();
};

#endif

// EOF
