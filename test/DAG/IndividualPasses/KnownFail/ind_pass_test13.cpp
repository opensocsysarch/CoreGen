//
// _IND_PASS_TEST13_CPP_
//
// Copyright (C) 130131-30131 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// CoreGen can not be copied and/or distributed without the
// express permission of Tactical Computing Laboratories, LLC
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST13";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "../TEST13.yaml";

std::string TARGETPASS = "DanglingNodePass";

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -1;
  }

  if( !CG->ReadIR( PROJYAML ) ){
    std::cout << "ERROR : FAILED TO READ YAML IR:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  if( !CG->BuildDAG() ){
    std::cout << "ERROR : FAILED TO BUILD THE DAG:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  if( !CG->InitPassMgr() ){
    std::cout << "ERROR : FAILED TO INIT THE PASS MANAGER:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  if( !CG->ExecutePass(TARGETPASS) ){
    std::cout << "ERROR : FAILED TO EXECUTE PASS : "
              << TARGETPASS << " : " << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
