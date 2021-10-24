//
// _SamplePlugin_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SamplePlugin
 *
 * \ingroup CoreGen
 *
 * \brief Sample CoreGenPlugin Implementation
 *
 */

#ifndef _SAMPLEPLUGIN_H_
#define _SAMPLEPLUGIN_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenPlugin/CoreGenPluginImpl.h"
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

// ---- Required Global Variables ----
///< SamplePlugin: [Required] plugin name (unique)
char __PLUGIN_NAME[]              = "SamplePlugin";

///< SamplePlugin: [Required] Plugin major version: MAJOR.MINOR.PATCH
unsigned __PLUGIN_MAJOR_VERSION   = 1;

///< SamplePlugin: [Required] Plugin minor version: MAJOR.MINOR.PATCH
unsigned __PLUGIN_MINOR_VERSION   = 0;

///< SamplePlugin: [Required] Plugin patch version: MAJOR.MINOR.PATCH
unsigned __PLUGIN_PATCH_VERSION   = 0;

///< SamplePlugin: [Required] Boolean to determine if this plugin has an HDL codegen
bool __PLUGIN_HAS_HDL_CODEGEN     = false;

///< SamplePlugin: [Required] Boolean to determine if this plugin has an LLVM codegen
bool __PLUGIN_HAS_LLVM_CODEGEN    = false;

class SamplePlugin : public CoreGenPluginImpl{
private:
public:

  /// Default constructor
  SamplePlugin(CoreGenEnv *EV, CoreGenErrno *E);

  /// Default destructor
  ~SamplePlugin();

  /// Process SamplePlugin Features
  bool ProcessFeatures() override;
};

#endif

// EOF
