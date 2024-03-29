//
// _CGCLIMain_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>
#include <vector>
#include <sys/param.h>
#include "CGCLIOpts.h"
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenArchive/CoreGenArchive.h"

std::string get_working_dir(){
  char buff[MAXPATHLEN];
  char *NStr = nullptr;
  NStr = getcwd( buff, MAXPATHLEN);
  if( NStr == nullptr ){
    return "/tmp";
  }
  std::string cwd( buff );
  return cwd;
}

int QueryArchive(CoreGenArchive *Archive,
                  CGCLIOpts* Opts){
  std::cout << "---------------------------------------------------------" << std::endl;
  std::cout << " CoreGen Archive Contents: " << Opts->GetArchiveFile() << std::endl;
  std::cout << "---------------------------------------------------------" << std::endl;
  for( unsigned i=0; i<Archive->GetNumEntries(); i++ ){
    CoreGenArchEntry *AE = Archive->GetEntry(i);
    std::cout << std::endl;
    std::cout << "Name        : " << AE->GetName() << std::endl;
    std::cout << "Directory   : " << AE->GetDirectory() << std::endl;
    std::cout << "Version     : " << AE->GetVersion() << std::endl;
    if( AE->IsLatest() ){
      std::cout << "Latest      : true" << std::endl;
    }else{
      std::cout << "Latest      : false" << std::endl;
    }
    std::cout << "Type        : " << AE->GetEntryTypeStr() << std::endl;
    std::cout << "Source Type : " << AE->GetSrcTypeStr() << std::endl;
    std::cout << "URL         : " << AE->GetURL() << std::endl;
    std::cout << "Postscript  : " << AE->GetPostscript() << std::endl;
    std::cout << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
  }
  return 0;
}

int InitArchive(CoreGenArchive *Archive,
                CGCLIOpts *Opts){
  int rtn = 0;
  if( Opts->GetArchiveEntry().length() > 0 ){
    // init an individual entry
    unsigned Entry = 0;
    if( !Archive->GetEntryNum(Opts->GetArchiveEntry(),
                              Entry) ){
      std::cout << "Error : no archive entry for "
                << Opts->GetArchiveEntry() << std::endl;
      return -1;
    }
    if( !Archive->Init(Entry) ){
      std::cout << "Error : " << Archive->GetErrStr() << std::endl;
      rtn = -1;
    }
  }else{
    // init all the entries
    if( !Archive->Init() ){
      std::cout << "Error : " << Archive->GetErrStr() << std::endl;
      rtn = -1;
    }
  }

  return rtn;
}

int DestroyArchive(CoreGenArchive *Archive,
                   CGCLIOpts *Opts){
  int rtn = 0;
  if( Opts->GetArchiveEntry().length() > 0 ){
    // destroy an individual entry
    unsigned Entry = 0;
    if( !Archive->GetEntryNum(Opts->GetArchiveEntry(),
                              Entry) ){
      std::cout << "Error : no archive entry for "
                << Opts->GetArchiveEntry() << std::endl;
      return -1;
    }
    if( !Archive->Destroy(Entry) ){
      std::cout << "Error : " << Archive->GetErrStr() << std::endl;
      rtn = -1;
    }
  }else{
    // init all the entries
    if( !Archive->Destroy() ){
      std::cout << "Error : " << Archive->GetErrStr() << std::endl;
      rtn = -1;
    }
  }

  return rtn;
}

int HandleArchive( CGCLIOpts *Opts ){
  CoreGenArchive *Archive = new CoreGenArchive( Opts->GetArchivePath() );
  if( Archive == nullptr ){
    std::cout << "Error : Could not create CoreGenArchive object" << std::endl;
    return -1;
  }

  // read in the archive file
  if( !Archive->ReadYaml(Opts->GetArchiveFile()) ){
    std::cout << "Error : " << Archive->GetErrStr() << std::endl;
    delete Archive;
    return -1;
  }

  int rtn = 0;

  if( Opts->IsArchiveQueryEnabled() ){
    rtn = QueryArchive(Archive,Opts);
  }else if( Opts->IsArchiveInitEnabled() ){
    rtn = InitArchive(Archive,Opts);
  }else if( Opts->IsArchiveDestroyEnabled() ) {
    rtn = DestroyArchive(Archive,Opts);
  }

  delete Archive;
  return rtn;
}

int ExecuteCoregen( CGCLIOpts *Opts ){
  CoreGenBackend *CG = new CoreGenBackend( Opts->GetProjectName(),
                                           Opts->GetProjectRoot(),
                                           Opts->GetArchivePath() );

  // look at the coregen version
  if( Opts->IsCoreGenVersion() ){
    int major = -1;
    int minor = -1;
    CG->CoreGenVersion( &major, &minor );
    std::cout << "Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC" << std::endl;
    std::cout << "CoreGen Version: " << major << "." << minor << std::endl;
    std::cout << "CoreGen Build Date: " << CG->CoreGenBuildDate()
      << " " << CG->CoreGenBuildTime() << std::endl;
    delete CG;
    return 0;
  }

  // verify the IR
  if( Opts->IsVerifyEnabled() ){
    if( !CG->ReadIR( Opts->GetIRFile() ) ){
      std::cout << "Error reading IR: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
    if( !CG->BuildDAG() ){
      std::cout << "Error building DAG: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // execute passes
  if( Opts->IsPassEnabled() ){
    if( !CG->InitPassMgr() ){
      std::cout << "Error initializing pass manager: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }

    if( !Opts->IsManualPasses() && !Opts->IsGroupPasses() ){
      // run all the passes
      if( !CG->ExecutePasses() ){
        std::cout << "Error executing full pass suite: "
                  << CG->GetErrStr() << std::endl;
        delete CG;
        return -1;
      }
    }else if( Opts->IsGroupPasses() ){
      if( Opts->IsDataPassesEnabled() ){
        if( !CG->ExecutePassType(CGPassData) ){
          std::cout << "Error executing Data passes: "
                    << CG->GetErrStr() << std::endl;
          delete CG;
          return -1;
        }
      }
      if( Opts->IsAnalysisPassesEnabled() ){
        if( !CG->ExecutePassType(CGPassAnalysis) ){
          std::cout << "Error executing Analysis passes: "
                    << CG->GetErrStr() << std::endl;
          delete CG;
          return -1;
        }
      }
      if( Opts->IsOptPassesEnabled() ){
        if( !CG->ExecutePassType(CGPassData) ){
          std::cout << "Error executing Optimization passes: "
                    << CG->GetErrStr() << std::endl;
          delete CG;
          return -1;
        }
      }
    }else{
      // run individual passes
      std::vector<std::string> Passes = CG->GetPasses();
      if( !Opts->CheckManualPassList(Passes) ){
        delete CG;
        return -1;
      }
      for( unsigned i=0; i<Passes.size(); i++ ){
        if( Opts->IsPassEnabled( Passes[i] ) ){
          if( !CG->ExecutePass( Passes[i] ) ){
            std::cout << "Error executing Pass=" << Passes[i]
                      << ": " << CG->GetErrStr() << std::endl;
            delete CG;
            return -1;
          }
        }
      }//end for i->Passes.size()
    }
  }// IsPassEnabled

  // check for system passes
  if( Opts->IsSysPassEnabled() ){
    if( !CG->InitPassMgr() ){
      std::cout << "Error initializing pass manager: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }

    for( unsigned i=0; i<Opts->GetNumSysPasses(); i++ ){
      // execute the i'th system pass
      if( Opts->GetSysPassIsArg(i) ){
        // set the argument
        if( !CG->SetPassOutputPath( Opts->GetSysPass(i),
                                    Opts->GetSysPassArg(i) ) ){
          std::cout << "Error setting output path for Pass= "
                    << Opts->GetSysPass(i)
                    << ": " << CG->GetErrStr() << std::endl;
          delete CG;
          return -1;
        }
      }

      if( !CG->ExecuteSysPass(Opts->GetSysPass(i)) ){
        std::cout << "Error executing system pass: "
                  << Opts->GetSysPass(i)
                  << ": " << CG->GetErrStr() << std::endl;
        delete CG;
        return -1;
      }
    }
  }

  // write the dot file
  if( Opts->IsDotEnabled() ){
    if( !CG->ExportDOT( Opts->GetDotFile() ) ){
      std::cout << "Error writing DOT file to " << Opts->GetDotFile() << ": "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // execute the codegen(s)
  if( Opts->IsChiselEnabled() && Opts->IsCompilerEnabled() ){
    // run both codegens (more efficient memory use)
    if( !CG->ExecuteCodegen() ){
      std::cout << "Error executing the codegens: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }else if( Opts->IsChiselEnabled() ){
    // run the chisel codegen
    if( !CG->ExecuteChiselCodegen() ){
      std::cout << "Error executing the Chisel codegen: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }else if( Opts->IsCompilerEnabled() ){
    // run the compiler codegen
    if( !CG->ExecuteLLVMCodegen(Opts->GetCompilerVersion()) ){
      std::cout << "Error executing the LLVM codegen: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }else if( Opts->IsStoneCutterEnabled() ){
    // run the stonecutter codegen
    if( !CG->ExecuteStoneCutterCodegen() ){
      std::cout << "Error executing the StoneCutter codegen: "
                << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  delete CG;
  return 0;
}

void ListPasses(){
  CoreGenBackend *CG = new CoreGenBackend( "CGCLI", "CGCLI", "CGCLI", CGSocProj );

  // insert a dummy node to make the dag happy
  CG->InsertISA( "dummy.isa" );
  if( !CG->BuildDAG() ){
    std::cout << "Error building dummy DAG : " << CG->GetErrStr() << std::endl;
    delete CG;
    return ;
  }
  if( !CG->InitPassMgr() ){
    std::cout << "Error initializing pass manager : " << CG->GetErrStr() << std::endl;
    delete CG;
    return ;
  }

  std::cout << "PASSNAME"
            << "                      "
            << "PASSTYPE"
            << "          "
            << "DESCRIPTION"
            << std::endl;
  std::cout << "--------------------------------------------------------------------------------" << std::endl;

  if( !CG->PrintPassInfo() ){
    std::cout << "Error printing pass info : " << CG->GetErrStr() << std::endl;
    delete CG;
    return ;
  }

  delete CG;
}

void ListSysPasses(){
  CoreGenBackend *CG = new CoreGenBackend( "CGCLI", "CGCLI", "CGCLI", CGSocProj );

  // insert a dummy node to make the dag happy
  CG->InsertISA( "dummy.isa" );
  if( !CG->BuildDAG() ){
    std::cout << "Error building dummy DAG : " << CG->GetErrStr() << std::endl;
    delete CG;
    return ;
  }
  if( !CG->InitPassMgr() ){
    std::cout << "Error initializing pass manager : " << CG->GetErrStr() << std::endl;
    delete CG;
    return ;
  }

  std::cout << "PASSNAME"
            << "                      "
            << "PASSTYPE"
            << "          "
            << "DESCRIPTION"
            << std::endl;
  std::cout << "--------------------------------------------------------------------------------" << std::endl;

  if( !CG->PrintSysPassInfo() ){
    std::cout << "Error printing pass info : " << CG->GetErrStr() << std::endl;
    delete CG;
    return ;
  }

  delete CG;
}

std::string FeatureTypeToString( CGFeatureType Type ){
  switch( Type ){
  case CGFUnsigned:
    return "Unsigned";
    break;
  case CGFUin32t:
    return "Uint32t";
    break;
  case CGFInt32t:
    return "Int32t";
    break;
  case CGFUint64t:
    return "Uint64t";
    break;
  case CGFInt64t:
    return "Int64t";
    break;
  case CGFFloat:
    return "Float";
    break;
  case CGFDouble:
    return "Double";
    break;
  case CGFString:
    return "String";
    break;
  case CGFBool:
    return "Bool";
    break;
  case CGFUnknown:
  default:
    return "UNKNOWN";
    break;
  }
}

int ExaminePlugins(CGCLIOpts *Opts){
  if( !Opts ){
    std::cout << "Error: null options" << std::endl;
    return -1;
  }

  CoreGenBackend *CG = new CoreGenBackend( "CGCLI", "CGCLI", "CGCLI", CGSocProj );

  std::cout << "PLUGIN NAME"
            << "                      "
            << "VERSION"
            << "                      "
            << "NUM FEATURES"
            << "                      " << std::endl
            << "--------------------------------------------------------------------------------" << std::endl;

  for( unsigned i=0; i<Opts->GetNumPlugins(); i++ ){
    if( !CG->LoadPlugin( Opts->GetPlugin(i) ) ){
      std::cout << "Error loading plugin : " << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }

    // read all the plugin details
    CoreGenPlugin *P = CG->GetPlugin(0);

    unsigned Major = 0;
    unsigned Minor = 0;
    unsigned Patch = 0;

    P->GetVersion(&Major,&Minor,&Patch);

    std::cout << P->GetPluginName()
              << "                      "
              << Major << "." << Minor << "." << Patch
              << "                      "
              << P->GetNumFeatures() << std::endl << std::endl
              << " -----> Features <-----" << std::endl;

    std::cout << "  Idx     Feature     Type" << std::endl;
    std::cout << "  ------------------------" << std::endl;
    for( unsigned j=0; j<P->GetNumFeatures(); j++ ){
      std::string FeatureType = FeatureTypeToString(P->GetFeatureType(j));
      std::string FeatureName = P->GetFeatureName(j);
      std::cout << "  [" << j << "]"
                << "     "
                << FeatureName
                << "     "
                << FeatureType
                << std::endl;
    }

    std::cout << std::endl << " -----> Codegen Options <-----" << std::endl;

    if( P->HasHDLCodegen() ){
      std::cout << "  HDL Codegen: yes" << std::endl;
    }else{
      std::cout << "  HDL Codegen: no" << std::endl;
    }

    if( P->HasLLVMCodegen() ){
      std::cout << "  LLVM Codegen: yes" << std::endl;
    }else{
      std::cout << "  LLVM Codegen: no" << std::endl;
    }

    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    if( !CG->ReleasePlugin( 0 ) ){
      std::cout << "Error releasing plugin : " << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  delete CG;

  return 0;
}

int main( int argc, char **argv ){

  // parse the options
  CGCLIOpts *Opts = new CGCLIOpts(argc,argv);
  if( !Opts->IsSuccess() ){
    delete Opts;
    return -1;
  }

  // check to see if help was selected
  if( Opts->IsHelp() ){
    delete Opts;
    return 0;
  }

  // check to see if we just need to list the passes
  if( Opts->IsListPasses() ){
    ListPasses();
    delete Opts;
    return 0;
  }

  // check to see if we just need to the list the sys passes
  if( Opts->IsListSysPasses() ){
    ListSysPasses();
    delete Opts;
    return 0;
  }

  // check to see if we need to handle the archive
  if( Opts->IsArchiveQueryEnabled() ||
      Opts->IsArchiveInitEnabled() ||
      Opts->IsArchiveDestroyEnabled() ){
    int rtn = HandleArchive(Opts);
    delete Opts;
    return rtn;
  }

  // check to see if we just need to examine some plugins
  if( Opts->IsCheckPluginsEnabled() ){
    int rtn = ExaminePlugins(Opts);
    delete Opts;
    return rtn;
  }

  // execute coregen
  int rtn = ExecuteCoregen(Opts);

  delete Opts;
  return rtn;
}
