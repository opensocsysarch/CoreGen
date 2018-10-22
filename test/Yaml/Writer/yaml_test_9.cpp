//
// _YAML_TEST9_CPP_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// CoreGen can not be copied and/or distributed without the
// express permission of Tactical Computing Laboratories, LLC
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST9";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "TEST9.yaml";

unsigned ITER=16;

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -1;
  }

  // add an SoC
  CoreGenSoC *SoC = CG->InsertSoC( PROJNAME + ".soc" );

  // add an ISA
  CoreGenISA *ISA[ITER];
  for( unsigned i=0; i<ITER; i++ ){
    ISA[i] = CG->InsertISA( PROJNAME + "." + std::to_string(i) + ".isa" );
  }

  // add a core
  CoreGenCore *Core[ITER];
  for( unsigned i=0; i<ITER; i++ ){
    Core[i] = CG->InsertCore( PROJNAME + "." + std::to_string(i) + ".core", ISA[i] );
  }

  SoC->InsertCore( Core[0] );

  // write the yaml
  if( !CG->WriteIR( PROJYAML ) ){
    std::cout << "ERROR : FAILED TO WRITE YAML IR:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
