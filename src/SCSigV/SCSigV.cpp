//
// _SCSigV_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>

#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

// ------------------------------------------------- PrintHelp
void PrintHelp(){
  std::cout << "scsigv [Options] /path/to/sigmap.yaml" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "     -h|-help|--help              : Print the help menu" << std::endl;
}

// ------------------------------------------------- ParseCommandLineOpts
bool ParseCommandLineOpts( int argc, char **argv,
                           bool &help, std::string &FName ){
  if( argc == 1 ){
    std::cout << "ERROR: No input files found" << std::endl;
    return false;
  }

  for( int i=1; i<argc; i++ ){
    std::string s(argv[i]);
    if( (s=="-h") || (s=="-help") || (s=="--help") ){
      help = true;
      return true;
    }else{
      FName = s;
    }
  }

  return true;
}

// ------------------------------------------------- main
int main( int argc, char **argv ){

  // validate the command line options
  bool help = false;
  std::string FName;

  if( !ParseCommandLineOpts(argc,argv,help,FName) ){
    return -1;
  }

  // print the help menu
  if( help ){
    PrintHelp();
    return 0;
  }

  CoreGenSigMap *SM = new CoreGenSigMap();
  if( !SM->ReadSigMap(FName) ){
    std::cout << "Failed to read signal map: " << FName << std::endl;
    delete SM;
    return -1;
  }else{
    std::cout << "Verified signal map: " << FName << std::endl;
  }

  delete SM;
  return 0;
}
