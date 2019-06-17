//
// _DELETE_NODE_TEST_1_CPP_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST1";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "../TEST1.yaml";
CGNodeType TYPE = CGSoc;

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
    std::cout << "ERROR : FAILED TO BUILD THE DAG: " << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  CoreGenNode *Top = CG->GetTop();
  if( Top == nullptr ){
    std::cout << "ERROR : FAILED TO RETRIEVE TOP NODE OF DAG: " << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == TYPE ){
      if( !CG->DeleteNode(Top->GetChild(i)) ){
        std::cout << "ERROR : FAILED TO DELETE NODE FROM DAG: " << CG->GetErrStr() << std::endl;
        delete CG;
        return -1;
      }
    }
  }

  if( !CG->BuildDAG() ){
    std::cout << "ERROR : FAILED TO BUILD THE DAG A SECOND TIME: " << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
