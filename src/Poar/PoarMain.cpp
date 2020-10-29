//
// _PoarMain_cpp__
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>

#include "CoreGen/Poar/PoarOpts.h"
#include "CoreGen/Poar/PoarConfig.h"
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/StoneCutter/StoneCutter.h"

int main( int argc, char **argv ){
  // -- classes --
  // options handler
  // configuration template handler
  // data handler
  // printer

  // Parse the command line options
  PoarOpts *POpts = new PoarOpts(argc,argv);
  if( !POpts ){
    std::cout << "Error : could not load Poar options" << std::endl;
    return -1;
  }

  bool isHelp = false;
  bool res = POpts->ParseOpts(&isHelp);
  if( isHelp ){
    delete POpts;
    return 0;
  }else if( !res ){
    // encountered an error
    delete POpts;
    return -1;
  }

  // Load a configuration template (or the default)
  PoarConfig *PConfig = new PoarConfig(POpts->GetConfig());
  if( !PConfig ){
    std::cout << "Error : could not load power/area configuration data" << std::endl;
    delete POpts;
    return -1;
  }

  // Load a coregen design object
  CoreGenBackend *CG = new CoreGenBackend();
  if( !CG ){
    std::cout << "Error : could not load CoreGen design data" << std::endl;
    delete CG;
    delete POpts;
    return -1;
  }

  if( !CG->ReadIR(POpts->GetDesignFile()) ){
    std::cout << "Error : could not read design input file : "
              << CG->GetErrStr() << std::endl;
    delete CG;
    delete POpts;
    return -1;
  }

  // Derive the power/area data

  // Print the data

  // Delete all the objects
  delete CG;
  delete POpts;

  return 0;
}
