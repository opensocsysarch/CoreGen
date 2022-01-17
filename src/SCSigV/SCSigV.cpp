//
// _SCSigV_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
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
  std::cout << "     -p|-pipeline|--pipeline      : Print pipeline stats" << std::endl;
  std::cout << "     -d|-datapath|--datapath      : Generate datapath diagram (dot graph)" << std::endl;
}

// ------------------------------------------------- ParseCommandLineOpts
bool ParseCommandLineOpts( int argc, char **argv,
                           bool &help, bool &stats, bool &pipeline,
                           bool &help, bool &stats, bool &pipeline, bool &datapath,
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
    }else if( (s=="-p") || (s=="-pipeline") || (s=="--pipeline") ){
      pipeline = true;
    }else if( (s=="-d") || (s=="-datapath") || (s=="--datapath") ){
      datapath = true;
    }else{
      FName = s;
    }
  }

  return true;
}

// ------------------------------------------------- PrintPipeline
void PrintPipeline( CoreGenSigMap *SM ){

  std::vector<std::string> Pipelines = SM->GetPipelines();

  std::cout << "---------------------------------------------------------------" << std::endl;
  std::cout << "Signal Map Pipeline Statistics" << std::endl;
  std::cout << "---------------------------------------------------------------" << std::endl;
  if( Pipelines.size() == 0 ){
    std::cout << "NO PIPELINES DEFINED!" << std::endl;
    return ;
  }

  std::cout << " Num Pipelines: " << Pipelines.size() << std::endl;
  std::cout << "---------------------------------------------------------------" << std::endl;

  for( unsigned i=0; i<Pipelines.size(); i++ ){
    std::cout << " Pipeline: " << Pipelines[i] << " contains "
              << SM->GetNumPipeStages(Pipelines[i]) << " pipeline stages" << std::endl;
    for( unsigned j=0; j<SM->GetNumPipeStages(Pipelines[i]); j++ ){
      std::cout << "    Stage [" << j << "] ==> "
                << SM->GetPipelineStage(Pipelines[i], j) << std::endl;
    }
  }

  std::cout << "---------------------------------------------------------------" << std::endl;
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

void GenDatapath(CoreGenSigMap* SM){
  std::cout << "digraph g {" << std::endl;                        // outermost container (entire system)
  unsigned NS = SM->GetNumSignals();                              // Get list of all signals
  std::vector<std::string> PipeNames = SM->GetPipelines();        // Get list of all pipelines

  for( unsigned i=0; i<PipeNames.size(); i++ ){                  // For each Pipeline 
    std::string Pipe = PipeNames[i];                             // Get Pipeline Name
    std::cout << "\tsubgraph " + Pipe + "{" << std::endl;        // Create subgraph for pipeline
    std::cout << "\t\tlabel = '" + Pipe + "';" << std::endl;     // Label it the name of pipeline
    unsigned NumStages = SM->GetNumPipeStages(Pipe); 
    for( unsigned j=0; j<NumStages; j++ ){                                         // for each stage inside pipeline
      std::string Stage = SM->GetPipelineStage(Pipe, j);                         // get current stage name
      std::vector<SCSig *> PipeSigs = SM->GetSignalVectByPipeStage(Pipe, Stage); // get all signals inside pipe stage
      std::cout << "\t\tsubgraph " + Stage + "{" << std::endl;                   // Make subgraph for stage sigs
      std::cout << "\t\t\tlabel = '" + Stage + "';" << std::endl;                // name the subgraph (stage name)
      std::string SignalString = "";
      for( unsigned k=0; k<PipeSigs.size(); k++ ){
        SignalString.append(PipeSigs[i]->GetName() + "->");                     // NOTE: Create separate bubbles for each signal 
      }
      std::cout << SignalString << std::endl;
      std::cout << "}" << std::endl;
    }
  std::cout << "}" << std::endl;
  } 
  std::cout << "}" << std::endl;
}

// ------------------------------------------------- main
int main( int argc, char **argv ){

  // validate the command line options
  bool help   = false;
  bool stats  = false;
  bool pipeline = false;
  bool datapath = false;
  std::string FName;

  if( !ParseCommandLineOpts(argc,argv,help,stats,pipeline, datapath, FName) ){
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
    std::cout << "Error = " << SM->GetErrStr() << std::endl;
    delete SM;
    return -1;
  }else{
    std::cout << "Verified signal map: " << FName << std::endl;
  }

  if( !SM->ExecutePasses() ){
    std::cout << "Failed to read signal map: " << FName << std::endl;
    std::cout << "Failed to execute signal map passes: " << SM->GetErrStr() << std::endl;
    delete SM;
    return -1;
  }

  
  if( stats ){
    PrintStats(SM);
  }

  if( pipeline ){
    PrintPipeline(SM);
  }

  if( datapath ){
    GenDatapath(SM);
  }

  delete SM;
  return 0;
}
