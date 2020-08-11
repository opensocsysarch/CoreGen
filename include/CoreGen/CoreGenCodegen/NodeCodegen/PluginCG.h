//
// _PluginCG_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PluginCG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenNodeCodegen derived class for plugins
 *
 */

#ifndef _PLUGINCG_H_
#define _PLUGINCG_H_

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

class PluginCG : public CoreGenNodeCodegen {
private:

public:
  /// Default constructor
  PluginCG(CoreGenNode *N,
         CoreGenProj *P,
         std::string Package,
         std::string Path,
         bool Common,
         CoreGenErrno *E );

  /// Default destructor
  ~PluginCG();

  /// Execute the codegen
  virtual bool Execute() { return false; }

  /// Execute the codegen
  bool Execute(CoreGenNode *Top, CoreGenNode *Parent);
};

#endif

// EOF
