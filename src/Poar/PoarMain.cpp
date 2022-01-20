//
// _PoarMain_cpp__
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>

#include "CoreGen/Poar/PoarData.h"
#include "CoreGen/Poar/PoarIO.h"

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

  PoarData *PData = new PoarData(POpts);
  if( !PData ){
    std::cout << "Error : could not load power/area data object" << std::endl;
    delete POpts;
    return -1;
  }

  if( !PData->Init() ){
    std::cout << "Error : " << PData->GetErrStr() << std::endl;
    delete POpts;
    delete PData;
    return -1;
  }

  // Derive the power/area data
  if( !PData->DeriveData() ){
    std::cout << "Error : " << PData->GetErrStr() << std::endl;
    delete POpts;
    delete PData;
    return -1;
  }

  // Print the data
  if( !PData->WriteData() ){
    std::cout << "Error : " << PData->GetErrStr() << std::endl;
    delete POpts;
    delete PData;
    return -1;
  }

  // Delete all the objects
  delete PData;
  delete POpts;

  return 0;
}
