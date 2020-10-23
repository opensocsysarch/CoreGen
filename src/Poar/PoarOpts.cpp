//
// _PoarOpts_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/PoarOpts.h"

PoarOpts::PoarOpts(int a, char **v)
  : argc(a), argv(v) { }

PoarOpts::~PoarOpts(){
}

void PoarOpts::PrintVersion(){
  std::cout << "Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC" << std::endl;
  std::cout << "POAR Version "
            << std::to_string(POAR_MAJOR_VERSION)
            << "." << std::to_string(POAR_MINOR_VERSION)
            << "." << std::to_string(POAR_PATCH_VERSION) << std::endl;
}

void PoarOpts::PrintHelp(){
  std::cout << "poar [Options] /path/to/design.yaml" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "\t-h|-help|--help                     : Print the help menu" << std::endl;
  std::cout << "\t-v|-version|--version                : Print the version information" << std::endl;
}

bool PoarOpts::ParseOpts(bool *isHelp){
  if( argc <= 1 ){
    std::cout << "Error : No input files found" << std::endl;
    return false;
  }

  // walk the args
  for( int i=1; i<argc; i++ ){
    std::string s(argv[i]);

    if( (s=="-h") || (s=="-help") || (s=="--help") ){
      *isHelp = true;
      PrintHelp();
      return true;
    }else if( (s=="-v") || (s=="-version") || (s=="--version") ){
      *isHelp = true;
      PrintVersion();
      return true;
    }else{
      std::cout << "Error : unknown option: " << s << std::endl;
      return false;
    }
  }

  return true;
}

// EOF
