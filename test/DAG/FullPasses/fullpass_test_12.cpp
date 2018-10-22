//
// _FULLPASS_TEST12_CPP_
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

std::string PROJNAME = "TEST12";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "TEST12.yaml";

unsigned ITER=16;

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT,CGSocProj);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -1;
  }

  // add an SoC
  CoreGenSoC *SoC = CG->InsertSoC( PROJNAME + ".soc" );

  // add an ISA
  CoreGenISA *ISA = CG->InsertISA( PROJNAME + ".isa" );

  // add a core
  CoreGenCore *Core = CG->InsertCore( PROJNAME + ".core", ISA );

  // add a register class
  CoreGenRegClass *RC = CG->InsertRegClass( PROJNAME + ".regclass" );

  // insert the regclass into the core
  if( !Core->InsertRegClass( RC ) ){
    std::cout << "ERROR : FAILED TO INSERT REGCLASS INTO CORE:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // add a register
  CoreGenReg *Reg = CG->InsertReg( PROJNAME + ".reg", 0, 64 );

  // insert the reg into the reg class
  if( !RC->InsertReg( Reg ) ){
    std::cout << "ERROR : FAILED TO INSERT REG INTO REGCLASS:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // insert the core in the SoC
  if( !SoC->InsertCore( Core ) ){
    std::cout << "ERROR : FAILED TO INSERT CORE INTO SOC:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // write the yaml
  if( !CG->WriteIR( PROJYAML ) ){
    std::cout << "ERROR : FAILED TO WRITE YAML IR:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // build the dag
  if( !CG->BuildDAG() ){
    std::cout << "ERROR : FAILED TO BUILD THE DAG:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // init the pass manager
  if( !CG->InitPassMgr() ){
    std::cout << "ERROR : FAILED TO INIT THE PASS MANAGER:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // execute the passes
  if( !CG->ExecutePasses() ){
    std::cout << "ERROR : FAILED TO EXECUTE DAG PASSES:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // export the DOT file
  if( !CG->ExportDOT(PROJNAME+".dot") ){
    std::cout << "ERROR : FAILED TO EXPORT THE DOT OUTPUT" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
