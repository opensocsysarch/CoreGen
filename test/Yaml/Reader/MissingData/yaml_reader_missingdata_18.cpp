//
// _YAML_READER_MISSINGDATA_18_CPP_
//
// Copyright (C) 2018-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
// 
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST18";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "../TEST18.yaml";

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -18;
  }

  if( !CG->ReadIR( PROJYAML ) ){
    std::cout << "ERROR : FAILED TO READ YAML IR: " << CG->GetErrStr() << std::endl;
    delete CG;
    return -18;
  }

  delete CG;
  return 0;
}
