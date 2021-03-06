//
// _YAML_TEST10_CPP_
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

std::string PROJNAME = "TEST10";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "TEST10.yaml";

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
    if( !SoC->InsertCore( Core[i] ) ){
      std::cout << "ERROR : FAILED TO INSERT CORE INTO SOC:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // write the yaml
  if( !CG->WriteIR( PROJYAML ) ){
    std::cout << "ERROR : FAILED TO WRITE YAML IR:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
