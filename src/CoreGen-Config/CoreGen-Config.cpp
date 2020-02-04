//
// _CoreGen-Config_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>

#define QuoteCGIPrefix(CGIPREFIX) #CGIPREFIX
#define QuoteMacro(macro) QuoteCGIPrefix(macro)

// ------------------------------------------------- PrintHelp
void PrintHelp(){
  std::cout << "coregen-config [Options]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "      -h|-help|--help          : Print the help menu" << std::endl;
  std::cout << "      -c|-compile|--compile    : Print the compilation string" << std::endl;
  std::cout << "      -i|-include|--include    : Print the include string" << std::endl;
  std::cout << "      -l|-libs|--libs          : Print the library string" << std::endl;
  std::cout << "      -L|-linkpath|--linkpath  : Print the linker path" << std::endl;
  std::cout << "      -p|-plugin|--plugin      : Print the plugin library string" << std::endl;
}

// ------------------------------------------------- InstallPrefix
std::string InstallPrefix(){
  return QuoteMacro(CGIPREFIX);
}

// ------------------------------------------------- ParseCommandLineOpts
bool ParseCommandLineOpts( int argc, char **argv,
                           bool &help, bool &compile,
                           bool &include, bool &libs,
                           bool &path, bool &plugin ){
  if( argc == 1 )
    return true;

  for( int i=1; i<argc; i++ ){
    std::string s(argv[i]);
    if( (s=="-h") || (s=="-help") || (s=="--help") ){
      help = true;
    }else if( (s=="-c") || (s=="-compile") || (s=="--compile") ){
      compile = true;
    }else if( (s=="-i") || (s=="-include") || (s=="--include") ){
      include = true;
    }else if( (s=="-l") || (s=="-libs") || (s=="--libs") ){
      libs = true;
    }else if( (s=="-L") || (s=="-linkpath") || (s=="--linkpath") ){
      path = true;
    }else if( (s=="-p") || (s=="-plugin") || (s=="--plugin") ){
      plugin = true;
    }
  }

  return true;
}

// ------------------------------------------------- Main
int main( int argc, char **argv ){
  bool help = false;
  bool compile = false;
  bool include = false;
  bool libs = false;
  bool path = false;
  bool plugin = false;

  if( !ParseCommandLineOpts(argc,argv,
                            help,compile,include,
                            libs,path,plugin) )
    return -1;

  // print the help menu
  if( help ){
    PrintHelp();
    return 0;
  }

  // print the compile string
  if( compile ){
    std::cout << "-I" << InstallPrefix()
              << " -L" << InstallPrefix() << "/lib"
              << " -lSCComp -lCoreGenBackend -lCoreGenArchive -lyaml-cpp"
              << std::endl;
  }

  // print the include string
  if( include ){
    std::cout << "-I" << InstallPrefix() << "/include" << std::endl;
  }

  // print the libs string
  if( libs ){
    std::cout << "-lSCComp -lCoreGenBackend -lCoreGenArchive -lyaml-cpp" << std::endl;
  }

  // print the path string
  if( path ){
    std::cout << "-L" << InstallPrefix() << "/lib" << std::endl;
  }

  // print the plugin string
  if( plugin ){
    std::cout << "-lCoreGenPluginImpl" << std::endl;
  }

  return 0;
}
