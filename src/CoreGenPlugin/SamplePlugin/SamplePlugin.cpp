//
// _SamplePlugin_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "SamplePlugin.h"

// ------------------------------------- BEGIN REQUIRED WRAPPER FUNCTIONS

// -------------------------------------
// Constructor Wrapper
// -------------------------------------
extern "C" CoreGenPluginImpl* __create(CoreGenErrno *E){
  return new SamplePlugin(E);
}

// -------------------------------------
// Destructor Wrapper
// -------------------------------------
extern "C" void __destroy(CoreGenPluginImpl *I){
  delete I;
}

// -------------------------------------
// Plugin Name Retrieval Wrapper
// -------------------------------------
extern "C" char *__getname(){
  return __PLUGIN_NAME;
}

// ------------------------------------- END REQUIRED WRAPPER FUNCTIONS

//
// SamplePlugin Constructor
//
// Inherits the CoreGenPluginImpl
// base class.  Initializes the base class
// with all the necessary version and configuration
// information.
//
SamplePlugin::SamplePlugin(CoreGenErrno *E)
  : CoreGenPluginImpl(std::string(__PLUGIN_NAME),   // Name of of the plugin
                      __PLUGIN_HAS_HDL_CODEGEN,     // Is there an overloaded HPL codegen?
                      __PLUGIN_HAS_LLVM_CODEGEN,    // Is there an overloaded LLVM codegen?
                      __PLUGIN_MAJOR_VERSION,       // Major version
                      __PLUGIN_MINOR_VERSION,       // Minor version
                      __PLUGIN_PATCH_VERSION,       // Patch version
                      E) {                          // Errno structure
    //
    // This is where we can initialize default values for
    // plugin-specific features
    //
    CGFeatureVal Val;
    Val.UnsignedData = 1;
    this->InsertFeature("CORES", CGFUnsigned, Val );

    Val.FloatData = 0.1f;
    this->InsertFeature("FEATURE2", CGFFloat, Val );

    Val.StringData = "THIS IS A STRING";
    this->InsertFeature("FEATURE3", CGFString, Val );
}


//
// SamplePlugin ProcessFeatures
//
// This function initializes all the internal
// features based upon what the user/frontend
// has defined in terms of the values.
// This is where plugins can perform custom
// error/range checking as well as any derived
// logic.
//
bool SamplePlugin::ProcessFeatures(){

  //
  // Purge all the children
  //
  // Plugins should do this before processing
  // features.  In this manner, you purge any
  // previous state that may poison the environment
  //
  if( !this->PurgeNodes() ){
    return false;
  }

  //
  // rebuild the node mappings
  //
  CoreGenISA *ISA = this->InsertISA( std::string(__PLUGIN_NAME) + ".isa" );

  for( unsigned i=0; i<this->GetFeatureValue("CORES").UnsignedData; i++ ){
    CoreGenCore *Core = this->InsertCore( std::string(__PLUGIN_NAME) + ".core" + std::to_string(i),
                                         ISA );
    if( !Core ){
      return false;
    }
  }

  return true;
}

//
// SamplePlugin Destructor
//
SamplePlugin::~SamplePlugin(){
}
