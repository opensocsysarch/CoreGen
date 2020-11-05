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
  : argc(a), argv(v),
    TextOutput(true), YamlOutput(false),
    LatexOutput(false), XmlOutput(false) { }

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
  std::cout << "poar [Options] --design /path/to/design.yaml" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "\t-h|-help|--help                     : Print the help menu" << std::endl;
  std::cout << "\t-v|-version|--version               : Print the version information" << std::endl;
  std::cout << "\t-c|-config|--config FILE.yaml       : Set the configuration file" << std::endl;
  std::cout << "\t-o|-outfile|--outfile FILE          : Sets the output file." << std::endl;
  std::cout << std::endl;
  std::cout << "Input Files:" << std::endl;
  std::cout << "\t--design /path/to/design.yaml       : Set the CoreGen design input file" << std::endl;
  std::cout << "\t--sigmap /path/to/sigmap.yaml       : Set the StoneCutter signal map" << std::endl;
  std::cout << std::endl;
  std::cout << "Output Formats:" << std::endl;
  std::cout << "\t--text                              : Text output (default)" << std::endl;
  std::cout << "\t--yaml                              : Yaml output" << std::endl;
  std::cout << "\t--latex                             : Latex output" << std::endl;
  std::cout << "\t--xml                               : XML output" << std::endl;
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
    }else if( (s=="-c") || (s=="-config") || (s=="--config") ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --config requires an argument" << std::endl;
        return false;
      }
      std::string S(argv[i+1]);
      Config = S;
      i++;
    }else if( (s=="-o") || (s=="-outfile") || (s=="--outfile") ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --outfile requires an argument" << std::endl;
        return false;
      }
      std::string S(argv[i+1]);
      OutFile = S;
      i++;
    }else if( s=="--design" ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --design requires an argument" << std::endl;
        return false;
      }
      std::string S(argv[i+1]);
      YamlFile = S;
      i++;
    }else if( s=="--sigmap" ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --sigmap requires an argument" << std::endl;
        return false;
      }
      std::string S(argv[i+1]);
      SCFile = S;
      i++;
    }else if( s=="--text" ){
      TextOutput = true;
      YamlOutput = false;
      LatexOutput = false;
      XmlOutput = false;
    }else if( s=="--yaml" ){
      TextOutput = false;
      YamlOutput = true;
      LatexOutput = false;
      XmlOutput = false;
    }else if( s=="--latex" ){
      TextOutput = false;
      YamlOutput = false;
      LatexOutput = true;
      XmlOutput = false;
    }else if( s=="--xml" ){
      TextOutput = false;
      YamlOutput = false;
      LatexOutput = false;
      XmlOutput = true;
    }else{
      std::cout << "Error : unknown option: " << s << std::endl;
      return false;
    }
  }

  if( (YamlFile.length() == 0) && (SCFile.length() == 0) ){
    std::cout << "Error : no design inputs specified; see the --design and --sigmap options" << std::endl;
    return false;
  }

  return true;
}

// EOF
