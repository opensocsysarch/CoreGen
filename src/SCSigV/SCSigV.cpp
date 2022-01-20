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
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

// ------------------------------------------------- PrintHelp
void PrintHelp(){
  std::cout << "scsigv [Options] /path/to/sigmap.yaml" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "     -h|-help|--help                  : Print the help menu" << std::endl;
  std::cout << "     -s|-stats|--stats                : Print stats" << std::endl;
  std::cout << "     -p|-pipeline|--pipeline          : Print pipeline stats" << std::endl;
  std::cout << "     -d|-datapath|--datapath file.dot : Generate datapath diagram (dot graph)" << std::endl;
  std::cout << "     --vliw                           : Generate VLIW signal map only" << std::endl;
}

// ------------------------------------------------- ParseCommandLineOpts
bool ParseCommandLineOpts( int argc, char **argv,
                           bool &help, bool &stats, bool &pipeline,
                           bool &datapath, bool &vliw,
                           std::string &DName,
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
      if( i+1 > (argc-1) ){
        std::cout << "Error : --datapath requires an argument" << std::endl;
        return false;
      }
      datapath = true;
      DName = argv[i+1];
      i++;
    }else if( s=="--vliw" ){
      vliw = true;
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

#define SIGNAME 0
#define DOTNAME 1
#define SIGTYPE 2

bool GenVLIWGraph(CoreGenSigMap* SM,
                  std::fstream &fs){
  std::vector<std::string> VLIWPipeStages = SM->GetVLIWPipeStages();
  std::vector<SCSig *> CSigs;

  std::vector<std::tuple<std::string,std::string,SigType>> NodeTuple;

  if( VLIWPipeStages.size() == 0 )
    return true;

  fs << "subgraph vliw {" << std::endl;
  for( unsigned i=0; i<VLIWPipeStages.size(); i++ ){
    // write a subgraph for the target stage
    fs << "subgraph cluster" << VLIWPipeStages[i] << " {" << std::endl;
    fs << "style=filled;" << std::endl;
    fs << "node [style=filled,color=white];" << std::endl;
    fs << "label = \"" << VLIWPipeStages[i] << "\";" << std::endl;

    CSigs = SM->GetVLIWSignalVectByPipeStage(VLIWPipeStages[i]);

    if( CSigs.size() > 0 ){
      // write all the normal nodes
      for( unsigned j=0; j<CSigs.size(); j++ ){
        if( (CSigs[j]->GetType() != DATA_IN) &&
            (CSigs[j]->GetType() != DATA_OUT) &&
            (CSigs[j]->GetType() != CTRL_IN) &&
            (CSigs[j]->GetType() != CTRL_OUT) ){
          fs << "node" << i << j << " [label=\"" << CSigs[j]->GetName() << "\"];" << std::endl;
        }
      }

      // write all the input/output nodes
      for( unsigned j=0; j<CSigs.size(); j++ ){
        if( (CSigs[j]->GetType() == DATA_IN) ||
            (CSigs[j]->GetType() == DATA_OUT) ||
            (CSigs[j]->GetType() == CTRL_IN) ||
            (CSigs[j]->GetType() == CTRL_OUT) ){
          fs << "vliwnode" << i << j
             << " [label=\"" << CSigs[j]->GetName() << "\",shape=Mdiamond];"
             << std::endl;
          NodeTuple.push_back( std::tuple<std::string,std::string,SigType>(
                                CSigs[j]->GetName(),
                                "vliwnode" + std::to_string(i) + std::to_string(j),
                                CSigs[j]->GetType()) );
        }
      }

      // we have to record the location of the first and last nodes
      // in order to ensure that we know which are the top and bottom
      // most nodes in the graph that are NOT IN/OUT signals
      unsigned FirstNode = 0;
      unsigned LastNode = 0;
      bool FN = false;
      // wire all the normal signals
      for( unsigned j=0; j<CSigs.size()-1; j++ ){
        if( (CSigs[j]->GetType() != DATA_IN) &&
            (CSigs[j]->GetType() != DATA_OUT) &&
            (CSigs[j]->GetType() != CTRL_IN) &&
            (CSigs[j]->GetType() != CTRL_OUT) &&
            (CSigs[j+1]->GetType() != DATA_IN) &&
            (CSigs[j+1]->GetType() != DATA_OUT) &&
            (CSigs[j+1]->GetType() != CTRL_IN) &&
            (CSigs[j+1]->GetType() != CTRL_OUT) ){
          fs << "node" << i << j << " -> " << "node" << i << j+1 << ";" << std::endl;
          if( !FN )
            FirstNode = j;
          LastNode = j+1;
        }
      }

      // wire the input signals
      for( unsigned j=0; j<CSigs.size(); j++ ){
        if( (CSigs[j]->GetType() == DATA_IN) ||
            (CSigs[j]->GetType() == CTRL_IN) ){
          fs << "vliwnode" << i << j << " -> " << "node" << i << FirstNode << ";" << std::endl;
        }
      }

      // wire the output signals
      for( unsigned j=0; j<CSigs.size(); j++ ){
        if( (CSigs[j]->GetType() == DATA_OUT) ||
            (CSigs[j]->GetType() == CTRL_OUT) ){
          fs << "node" << i << LastNode << " -> " << "vliwnode" << i << j << ";" << std::endl;
        }
      }

      // wire the I/O signals together
      for( unsigned j=0; j<NodeTuple.size(); j++ ){
        if( (std::get<SIGTYPE>(NodeTuple[j]) == DATA_OUT) ||
            (std::get<SIGTYPE>(NodeTuple[j]) == CTRL_OUT) ){
          // found an output signal, find the complementary input signal
          for( unsigned k=0; k<NodeTuple.size(); k++ ){
            if( ((std::get<SIGTYPE>(NodeTuple[k]) == DATA_IN) ||
                (std::get<SIGTYPE>(NodeTuple[k]) == CTRL_IN)) &&
                (std::get<SIGNAME>(NodeTuple[j]) ==
                                  std::get<SIGNAME>(NodeTuple[k])) ){
                // wire to the I/O
                fs << std::get<DOTNAME>(NodeTuple[j]) << " -> "
                   << std::get<DOTNAME>(NodeTuple[k]) << ";" << std::endl;
            }
          }
        }
      }
    }

    CSigs.clear();

    fs << "}" << std::endl;
  }
  fs << "}" << std::endl;
  return true;
}

bool GenRISCGraph(CoreGenSigMap* SM,
                  std::fstream &fs){
  return true;
}

bool GenDatapath(CoreGenSigMap* SM, bool vliw,
                 std::string DName){

  std::fstream fs;

  // open the output file
  fs.open(DName, std::fstream::out);
  if( !fs.is_open() ){
    std::cout << "Error : Could not open output file: " << DName << std::endl;
    return false;
  }

  fs << "#" << std::endl;
  fs << "# Generate graph using \" dot  -Tpdf thisfile > thisfile.pdf\"" << std::endl;
  fs << "#" << std::endl << std::endl;

  // inject the header info
  fs << "digraph G{" << std::endl;
  fs << "compound=true;" << std::endl;

  if( vliw ){
    if( !GenVLIWGraph(SM,fs) ){
      fs.close();
      return false;
    }
  }else{
    if( !GenRISCGraph(SM,fs) ){
      fs.close();
      return false;
    }
  }

  // inject the footer info
  fs << "}" << std::endl;

  // close the output file
  fs.close();
  return true;
}

// ------------------------------------------------- main
int main( int argc, char **argv ){

  // validate the command line options
  bool help     = false;
  bool stats    = false;
  bool pipeline = false;
  bool datapath = false;
  bool vliw     = false;
  std::string FName;
  std::string DName;

  if( !ParseCommandLineOpts(argc,argv,help,stats,pipeline,
                            datapath, vliw, DName, FName) ){
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
    if( !GenDatapath(SM,vliw,DName) ){
      delete SM;
      return -1;
    }
  }

  delete SM;
  return 0;
}
