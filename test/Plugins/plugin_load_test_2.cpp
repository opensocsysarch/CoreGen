//
// _PLUGIN_LOAD_TEST_2_CPP
//
// Copyright (C) 2027-2028 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
// 
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST2";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -2;
  }

  std::string PLUGIN = std::string(argv[1]);

  if( !CG->LoadPlugin(PLUGIN) ){
    std::cout << "ERROR : FAILED TO LOAD PLUGIN : " << PLUGIN << std::endl;
    delete CG;
    return -1;
  }

  if( !CG->ReleasePlugin(0) ){
    std::cout << "ERROR : FAILED TO RELEASE PLUGIN : " << PLUGIN << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
