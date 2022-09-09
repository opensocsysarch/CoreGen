//
// _DHDTOpts_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/DHDT/DHDTOpts.h"

DHDTOpts::DHDTOpts(int a, char **v)
  : argc(a), argv(v),
    isHazard(false), isPower(false), isVerbose(false) {}

DHDTOpts::~DHDTOpts(){}

void DHDTOpts::PrintVersion(){
  std::cout << "Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC"
            << std::endl;
  std::cout << "DHDT Version "
            << std::to_string(DHDT_MAJOR_VERSION)
            << "." << std::to_string(DHDT_MINOR_VERSION)
            << "." << std::to_string(DHDT_PATCH_VERSION)
            << std::endl;
}

void DHDTOpts::PrintHelp(){
  std::cout << "dhdt [Options] [Runtime Options] --llvm /path/to/llvm.ir --coregen /path/to/design.yaml" << std::endl;
  std::cout << "[Options]" << std::endl;
  std::cout << "\t-h|-help|--help                   : Print the help menu" << std::endl;
  std::cout << "\t-o|-output|--output               : Output file for reports" << std::endl;
  std::cout << "\t-d|-dot|--dot /path/to/file.dot   : Output the LLVM IR graph in DOT format" << std::endl;
  std::cout << "\t-i|-inst|--inst /path/to/inst.txt : Instruction input" << std::endl;
  std::cout << "[Runtime Options]" << std::endl;
  std::cout << "\t-hazard|--hazard                  : Enable hazard discovery tests" << std::endl;
  std::cout << "\t-power|--power [/path/to/power]   : Enable power discovery tests" << std::endl;
  std::cout << "\t-verbose|--verbose                : Enable verbosity" << std::endl;
  std::cout << std::endl;
}

bool DHDTOpts::ParseOpts(bool &isHelp){
  if( argc <= 1 ){
    PrintHelp();
    return false;
  }

  for( int i=1; i<argc; i++ ){
    std::string s(argv[i]);

    if( (s=="-h") || (s=="-help") || (s=="--help") ){
      isHelp = true;
      PrintHelp();
      return true;
    }else if( (s=="-verbose") || (s=="--verbose") ){
      isVerbose = true;
    }else if( (s=="-hazard") || (s=="--hazard") ){
      isHazard = true;
    }else if( (s=="-power") || (s=="--power") ){
      isPower = true;

      // determine if there is an optional argument
      if( i+1 <= (argc-1) ){
        std::string TmpS(argv[i+1]);
        if( TmpS[0] != '-' ){
          // found the next argument is not a CLI option
          PFile = TmpS;
          i++;
        }
      }
    }else if( s == "--llvm" ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --llvm requires an argument" << std::endl;
        return false;
      }
      std::string TmpS(argv[i+1]);
      IRFile = TmpS;
      i++;
    }else if( s == "--coregen" ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --coregen requires an argument" << std::endl;
        return false;
      }
      std::string TmpS(argv[i+1]);
      CFile = TmpS;
      i++;
    }else if( (s=="-o") || (s=="-output") || (s=="--output") ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --output requires an argument" << std::endl;
        return false;
      }
      std::string TmpS(argv[i+1]);
      OutFile = TmpS;
      i++;
    }else if( (s=="-i") || (s=="-inst") || (s=="--inst") ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --inst requires an argument" << std::endl;
        return false;
      }
      std::string TmpS(argv[i+1]);
      InstFile = TmpS;
      i++;
    }else if( (s=="-d") || (s=="-dot") || (s=="--dot") ){
      if( i+1 > (argc-1) ){
        std::cout << "Error : --dot requires an argument" << std::endl;
        return false;
      }
      std::string TmpS(argv[i+1]);
      DotFile = TmpS;
      i++;
    }else{
      std::cout << "Error : unknown option: " << s << std::endl;
      return false;
    }
  }

  if( (IRFile.length() == 0) || (CFile.length() == 0) ){
    std::cout << "Error : LLVM IR and CoreGen IR files are required" << std::endl;
    std::cout << "      : see the --llvm and --coregen options" << std::endl;
    return false;
  }

  return true;
}

// EOF
