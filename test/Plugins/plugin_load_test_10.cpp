//
// _PLUGIN_LOAD_TEST_10_CPP
//
// Copyright (C) 1001010-1001010 Tactical Computing Laboratories, LLC
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

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -10;
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

    CGFeatureType FType = P->GetFeatureType(i);
    std::string FTypeName;
    switch( FType ){
    case CGFUnsigned:
      FTypeName =  "Unsigned";
      break;
    case CGFUin32t:
      FTypeName =  "Uint32t";
      break;
    case CGFint32t:
      FTypeName =  "Int32t";
      break;
    case CGFUint64t:
      FTypeName =  "Uint64t";
      break;
    case CGFInt64t:
      FTypeName =  "Int64t";
      break;
    case CGFFloat:
      FTypeName =  "Float";
      break;
    case CGFDouble:
      FTypeName =  "Double";
      break;
    case CGFString:
      FTypeName =  "String";
      break;
    case CGFBool:
      FTypeName =  "Bool";
      break;
    case CGFUnknown:
    default:
      FTypeName =  "UNKNOWN";
      break;
    }
    std::cout << "Found feature type : " << FTypeName << std::endl;
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
