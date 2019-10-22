//
// _CGCLIOpts_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <unistd.h>
#include <algorithm>
#include "CGCLIConfig.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

class CGCLIOpts
{
private:
  bool ParseSuccess;    ///< CGCLIOpts: Was the parsing successsful
  bool HelpSelected;    ///< CGCLIOpts: Was 'help' selected
  bool ManualPasses;    ///< CGCLIOpts: Were passes selected manually
  bool GroupPasses;     ///< CGCLIOpts: Enable groups of passes
  bool DataPasses;      ///< CGCLIOpts: Enable the data passes
  bool AnalysisPasses;  ///< CGCLIOpts: Enable the analysis passes
  bool OptPasses;       ///< CGCLIOtps: Enable the optimization passses
  bool ListPasses;      ///< CGCLIOpts: Did the user request to list the passes
  bool Verify;          ///< CGCLIOpts: Verify the IR
  bool ExecPass;        ///< CGCLIOpts: Execute any or all passes
  bool CGChisel;        ///< CGCLIOpts: Run the Chisel codegen
  bool CGComp;          ///< CGCLIOpts: Run the compiler codegen
  bool CGVersion;       ///< CGCLIOpts: Print CoreGen version
  bool CheckPlugins;    ///< CGCLIOpts: Check the plugin options
  bool ListSysPasses;   ///< CGCLIOpts: Did the user request to list the sys passes
  bool ExecSysPass;     ///< CGCLIOpts: Execute a system pass
  bool ASPSolver;       ///< CGCLIOpts: Execute the ASP Solver
  bool ASPClean;        ///< CGCLIOpts: Clean up the ASP DAG file
  bool ArchQuery;       ///< CGCLIOpts: Query the archive
  bool ArchInit;        ///< CGCLIOpts: Init the archive
  bool ArchDestroy;     ///< CGCLIOpts: Destroy the archive

  std::string IRFile;   ///< CGCLIOpts: Input IR File
  std::string OutFile;  ///< CGCLIOpts: Output IR File
  std::string DotFile;  ///< CGCLIOpts: DOT output File
  std::string ProjName; ///< CGCLIOpts: Project Name
  std::string ProjRoot; ///< CGCLIOpts: Project Root
  std::string Archive;  ///< CGCLIOpts: Archive Path
  std::string ArchFile; ///< CGCLIOpts: Archive File
  std::string ArchEntry;///< CGCLIOpts: Archive Entry

  std::vector<std::string> EnablePass;  ///< CGCLIOpts: Manually enabled passes
  std::vector<std::string> DisablePass; ///< CGCLIOpts: Manually disabled passes
  std::vector<std::string> Plugins;     ///< CGCLIOpts: Plugins for checking
  std::vector<std::string> ASPFiles;

  std::vector<std::tuple<std::string,bool,std::string>> EnableSysPass; ///< CGCLIOpts: Enabled System Passes

  /// Parse the options
  bool ParseOpts(int argc, char **argv);

  /// Print the options
  void PrintOptions();

  /// Parse the plugin options
  std::vector<std::string> ParsePlugins( std::string Plugins);

  /// Parse the pass options
  std::vector<std::string> ParsePasses( std::string Passes );

  /// Parse the system pass options
  bool ParseSysPasses(std::string Passes);

  /// Split a string into pass tokens
  void Split(const std::string& s, char delim,
             std::vector<std::string>& v);

  /// Set the default paths
  void SetDefaultPaths();

  /// Tests whether the input file exists
  bool FileExists(std::string F);

public:
  /// Default constructor
  CGCLIOpts( int argc, char **argv );

  /// Default destructor
  ~CGCLIOpts();

  /// Check if parsing was successful
  bool IsSuccess() { return ParseSuccess; }

  /// Check if help option was selected
  bool IsHelp() { return HelpSelected; }

  /// Check if the CoreGen version option was selected
  bool IsCoreGenVersion() { return CGVersion; }

  /// Check if list passes option was selected
  bool IsListPasses() { return ListPasses; }

  /// Check if the list sys passes option was selected
  bool IsListSysPasses() { return ListSysPasses; }

  /// Check if the verification option is enabled
  bool IsVerifyEnabled() { return Verify; }

  /// Check if the pass execution is enabled
  bool IsPassEnabled() { return ExecPass; }

  /// Check if the dot file output is enabled
  bool IsDotEnabled() { if( DotFile.length() > 0 ){ return true; } return false; }

  /// Check if the chisel codegen is enabled
  bool IsChiselEnabled() { return CGChisel; }

  /// check if the compiler codegen is enabled
  bool IsCompilerEnabled() { return CGComp; }

  /// Check to see if manual pass control is enabled
  bool IsManualPasses() { return ManualPasses; }

  /// Check to see if target pass is enabled
  bool IsPassEnabled( std::string Pass );

  /// Check to see if system passes are enabled
  bool IsSysPassEnabled() { return ExecSysPass; }

  // Check to see if the ASP pass is enabled
  bool IsASPEnabled() { return ASPSolver; }

  /// Check to see if the group passes are enabled
  bool IsGroupPasses() { return GroupPasses; }

  /// Check to see if the data passes are enabled
  bool IsDataPassesEnabled() { return DataPasses; }

  /// Check to see if the analysis passes are enabled
  bool IsAnalysisPassesEnabled() { return AnalysisPasses; }

  /// Check to see if the optimization passes are enabled
  bool IsOptPassesEnabled() { return OptPasses; }

  /// Check to see if the check plugins option is enabled
  bool IsCheckPluginsEnabled() { return CheckPlugins; }

  /// Check if ASP DAG needs to bee cleaned up
  bool CleanASP() { return ASPClean; }

  /// Retrieve the number of selected plugins
  unsigned GetNumPlugins() { return Plugins.size(); }

  /// Retrieve the ASP input string
  std::vector<std::string> GetASPFiles() { return ASPFiles; }

  /// Retrieve the target plugin name
  std::string GetPlugin(unsigned P){
    if( P > (Plugins.size()-1) ){
      return "";
    }
    return Plugins[P];
  }

  /// Retrieve the number of enabled system passes
  unsigned GetNumSysPasses() { return EnableSysPass.size(); }

  /// Retrieve the target system pass
  std::string GetSysPass(unsigned Pass);

  /// Retrieve the target system pass arguments
  std::string GetSysPassArg(unsigned Pass);

  /// Retrieve whether or not a system pass has arguments
  bool GetSysPassIsArg(unsigned Pass);

  /// Check the list of parsed manually disabled/enabled passes against CoreGen
  bool CheckManualPassList(std::vector<std::string> Passes);

  /// Retrieve the closest pass name to the errorneous pass name provided
  std::string GetNearbyString(std::string &Input,
                              std::vector<std::string> Passes);

  /// Derive the edit distance between two strings
  unsigned EditDistance( std::string &s1, std::string &s2 );

  /// Retrieve the dot file name
  std::string GetDotFile() { return DotFile; }

  /// Retrieve the IR file name
  std::string GetIRFile() { return IRFile; }

  /// Retrieve the project name
  std::string GetProjectName() { return ProjName; }

  /// Retrieve the project root
  std::string GetProjectRoot() { return ProjRoot; }

  /// Retrieve the project archive path
  std::string GetArchivePath() { return Archive; }
};

// EOF
