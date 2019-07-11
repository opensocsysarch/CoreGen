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
#include <iomanip>
#include <algorithm>
#include <string>

#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

// ------------------------------------------------- PrintHelp
void PrintHelp(){
  std::cout << "scsigv [Options] /path/to/sigmap.yaml" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "     -h|-help|--help              : Print the help menu" << std::endl;
  std::cout << "     -s|-stats|--stats            : Print stats" << std::endl;
}

// ------------------------------------------------- ParseCommandLineOpts
bool ParseCommandLineOpts( int argc, char **argv,
                           bool &help, bool &stats,
                           std::string &FName ){
  if( argc == 1 ){
    std::cout << "ERROR: No input files found" << std::endl;
    return false;
  }

  for( int i=1; i<argc; i++ ){
    std::string s(argv[i]);
    if( (s=="-h") || (s=="-help") || (s=="--help") ){
      help = true;
      return true;
    }else if( (s=="-s") || (s=="-stats") || (s=="--stats") ){
      stats = true;
    }else{
      FName = s;
    }
  }

  return true;
}

// ------------------------------------------------- PrintStats
void PrintStats( CoreGenSigMap *SM ){
  unsigned NS = SM->GetNumSignals();

  std::cout << "---------------------------------------------------------------" << std::endl;
  std::cout << "Signal Map Statistics" << std::endl;
  std::cout << "---------------------------------------------------------------" << std::endl;
  std::cout << " Num Signals : " << NS << std::endl;
  std::cout << "---------------------------------------------------------------" << std::endl;
  std::cout << "                      Unique Signals" << std::endl;
  std::cout << "Signal"   << std::setw(15)
            << "Num Uses" << std::setw(10)
            << "MinWidth" << std::setw(10)
            << "MaxWidth" << std::endl;

  std::vector<SigType> Types;
  for( unsigned i=0; i<NS; i++ ){
    Types.push_back(SM->GetSignal(i)->GetType());
  }
  std::sort( Types.begin(), Types.end() );
  Types.erase( std::unique(Types.begin(),Types.end()), Types.end() );

  for(unsigned i=0; i<Types.size(); i++ ){
    // gather all the stats
    unsigned Min = 0;
    unsigned Max = 0;
    unsigned Count = 0;
    std::string SigStr;
    for( unsigned j=0; j<NS; j++ ){
      if( SM->GetSignal(j)->GetType() == Types[i] ){
        SigStr  = SM->GetSignal(j)->SigTypeToStr();
        Min     = SM->GetMinSignalWidth(j);
        Max     = SM->GetMaxSignalWidth(j);
        Count   = Count + 1;
      }
    }

    // print everything
    std::cout << SigStr << std::setw( (15-SigStr.length())+3 )
              << Count  << std::setw(10)
              << Min    << std::setw(10)
              << Max    << std::endl;
  }

  std::cout << "---------------------------------------------------------------" << std::endl;
}

// ------------------------------------------------- main
int main( int argc, char **argv ){

  // validate the command line options
  bool help   = false;
  bool stats  = false;
  std::string FName;

  if( !ParseCommandLineOpts(argc,argv,help,stats,FName) ){
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

  if( stats ){
    PrintStats(SM);
  }

  delete SM;
  return 0;
}
