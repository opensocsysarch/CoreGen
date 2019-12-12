//
// _PLUGIN_LOAD_TEST_9_CPP
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
// 
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST9";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -9;
  }

  std::string PLUGIN = std::string(argv[1]);

  if( !CG->LoadPlugin(PLUGIN) ){
    std::cout << "ERROR : FAILED TO LOAD PLUGIN : " << PLUGIN << std::endl;
    delete CG;
    return -1;
  }

  CoreGenPlugin *P = CG->GetPlugin("SamplePlugin");
  if( !P ){
    std::cout << "ERROR : FAILED TO RETRIEVE PLUGIN POINTER : " << PLUGIN << std::endl;
    delete CG;
    return -1;
  }

  std::string PName = P->GetPluginName();
  if( PName.length() == 0 ){
    std::cout << "ERROR : FAILED TO RETRIEVE PLUGIN NAME : " << PLUGIN << std::endl;
    delete CG;
    return -1;
  }

  unsigned NF = P->GetNumFeatures();
  if( NF == 0 ){
    std::cout << "ERROR : FAILED TO RETRIEVE THE NUMBER OF FEATURES : " << PLUGIN << std::endl;
    delete CG;
    return -1;
  }

  for( unsigned i=0; i<NF; i++ ){
    std::string FName = P->GetFeatureName(i);
    std::cout << "Found feature : " << FName << std::endl;
  }

  std::cout << "PLUGIN NAME = " << PName << " WITH " << NF << " FEATURES" << std::endl;

  if( !CG->ReleasePlugin("SamplePlugin") ){
    std::cout << "ERROR : FAILED TO RELEASE PLUGIN : " << PLUGIN << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
