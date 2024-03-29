//
// _SCOpts_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCOpts
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter compiler options class
 *
 */

#ifndef _STONECUTTER_SCOPTS_H_
#define _STONECUTTER_SCOPTS_H_

// Standard Headers
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <streambuf>

#include <fstream>
#include <sstream>
#include <algorithm>

// StoneCutter Headers
#include "CoreGen/StoneCutter/SCMsg.h"

#ifndef SC_MAJOR_VERSION
#define SC_MAJOR_VERSION 0
#endif

#ifndef SC_MINOR_VERSION
#define SC_MINOR_VERSION 1
#endif

#ifndef SC_PATCH_VERSION
#define SC_PATCH_VERSION 1
#endif

class SCOpts{
private:
  // private variables
  int argc;         ///< SCOpts: ARGC command line info
  char **argv;      ///< SCOpts: ARGV command line info

  bool isKeep;      ///< SCOpts: Keep intermediate files
  bool isParse;     ///< SCOpts: Parse the incoming input
  bool isIR;        ///< SCOpts: Generate IR
  bool isOptimize;  ///< SCOpts: Execute the optimizer
  bool isChisel;    ///< SCOpts: Execute the chisel codegen
  bool isCG;        ///< SCOpts: Execute the object codegen
  bool isVerbose;   ///< SCOpts: Enable verbosity
  bool isDisable;   ///< SCOpts: Manually disabled passes flag
  bool isEnable;    ///< SCOpts: Manually enabled  passes flag
  bool isListPass;  ///< SCOpts: User selected pass listing flag
  bool isListSCPass;///< SCOpts: User selected StoneCutter pass listing flag
  bool isSigMap;    ///< SCOpts: Signal map selected
  bool isPassRun;   ///< SCOpts: Have the passes been executed?
  bool isPerf;      ///< SCOpts: Are the performance stats enabled?
  bool isSCDisable; ///< SCOpts: Manually disabled StoneCutter passes flag
  bool isSCEnable;  ///< SCOpts: Manually enabled StoneCutter passes flag
  bool isPipeline;  ///< SCOpts: Is the pipeline optimizer enabled?

  bool isAreaOpt;   ///< SCOpts: Is the area pipeline optimizations enabled
  bool isPowerOpt;  ///< SCOpts: Is the power pipeline optimizations enabled
  bool isPerfOpt;   ///< SCOpts: Is the performance pipeline optimizations enabled

  std::string OutFile;  ///< SCOpts: Output file designator
  std::string SigMap;   ///< SCOpts: Signal map output file

  std::string Package;  ///< SCOpts: Chisel package name
  std::string ISA;      ///< SCOpts: Chisel ISA name

  std::vector<std::string> FileList;      ///< SCOpts: List of files to compile

  std::vector<std::string> EnablePass;    ///< SCOpts: List of enabled passes
  std::vector<std::string> DisablePass;   ///< SCOpts: List of disabled passes

  std::vector<std::string> EnableSCPass;  ///< SCOpts: List of enabled StoneCutter passes
  std::vector<std::string> DisableSCPass; ///< SCOpts: List of disabled StoneCutter passes

  std::map<std::string,std::string> SCPassOpts;   ///< SCOpts: Map of StoneCutter pass to respective options
  std::map<std::string,std::string> PipePassOpts; ///< SCOpts: Map of Pipeline pass options to values

  SCMsg *Msgs;    ///< SCOpts: Message handlers

  // private functions
  /// Print the help menu
  void PrintHelp();

  /// Print the version information
  void PrintVersion();

  /// Parse the pass options and split them into a vector
  std::vector<std::string> ParsePasses(std::string P);

  /// Parse the StoneCutter pass name:options strings and split them into a map
  std::map<std::string,std::string> ParsePassOpts(std::string P);

  /// Split the options list by commas
  void Split(const std::string &s, char delim,
             std::vector<std::string>& v);

  /// Determines if a dash is found in the input name
  bool FindDash(const std::string &s);

  /// Determines if a pipeline option is found
  bool FindPipeline(const std::string &s);

  /// Parses the pipeline options string
  bool ParsePipelineOpts(std::string P);

  /// Derives the ISA name from the path
  std::string GetISANameFromPath();

  /// Removes any dots in the ISA name
  std::string SCRemoveDot(std::string);

  /// Removes the extension from the ISA name
  std::string SCRemoveExtension(std::string filname);

public:
  /// SCOpts: Constructor
  SCOpts(SCMsg *);

  /// SCOpts: Overloaded constructor
  SCOpts(SCMsg *,int,char **);

  /// SCOpts: Desctructor
  ~SCOpts();

  /// SCOpts: Parse the command line options
  bool ParseOpts(bool *isHelp);

  /// SCOpts: Keep the intermediate files?
  bool IsKeep() { return isKeep; }

  /// SCOpts: Do we generate IR
  bool IsIR() { return isIR; }

  /// SCOpts: Do we execute the codegen
  bool IsCG() { return isCG; }

  /// SCOpts: Do we generate chisel output
  bool IsChisel() { return isChisel; }

  /// SCOpts: Are the performance stats enabled
  bool IsPerf() { return isPerf; }

  /// SCOpts: Do we execute the parser
  bool IsParse() { return isParse; }

  /// SCOpts: Do we have manually disabled passes
  bool IsDisablePass() { return isDisable; }

  /// SCOpts: Do we have manually enabled passes
  bool IsEnablePass() { return isEnable; }

  /// SCOpts: Do we have manually disabled StoneCutter passes
  bool IsDisableSCPass() { return isSCDisable; }

  /// SCOpts: Do we have manually enabled StoneCutter passes
  bool IsEnableSCPass() { return isSCEnable; }

  /// SCOpts: Do we need to list the LLVM passes
  bool IsListPass() { return isListPass; }

  /// SCOpts: Do we need to list the StoneCutter passes
  bool IsListSCPass() { return isListSCPass; }

  /// SCOpts: Do we have StoneCutter pass-specific options
  bool IsSCPassOpt() { return !SCPassOpts.empty(); }

  /// SCOpts: Do we execute the optimizer
  bool IsOptimize() { return isOptimize; }

  /// SCOpts: Is the pipeline optimizer enabled
  bool IsPipeline() { return isPipeline; }

  /// SCOpts: Are the area optimizations enabled
  bool IsAreaOpt() { return isAreaOpt; }

  /// SCOpts: Are the power optimizations enabled
  bool IsPowerOpt() { return isPowerOpt; }

  /// SCOpts: Are the performance optimizations enabled
  bool IsPerfOpt() { return isPerfOpt; }

  /// SCOpts: Is verbosity enabled?
  bool IsVerbose() { return isVerbose; }

  /// SCOpts: Is signal map enabled?
  bool IsSignalMap() { return isSigMap; }

  /// SCOpts: Have the passes been executed()?
  bool IsPassRun() { return isPassRun; }

  /// SCOpts: Force the passes to rerun
  void ReRunPass() { isPassRun = false; }

  /// SCOpts: Notify the options object that the passes have been run
  void PassRun() { isPassRun = true; }

  /// SCOpts: Retrieve the list of disabled passes
  std::vector<std::string> GetDisabledPass() { return DisablePass; }

  /// SCOpts: Rerieve the list of enabled passes
  std::vector<std::string> GetEnablePass() { return EnablePass; }

  /// SCOpts: Retrieve the list of disabled StoneCutter passes
  std::vector<std::string> GetDisabledSCPass() { return DisableSCPass; }

  /// SCOpts: Rerieve the list of enabled StoneCutter passes
  std::vector<std::string> GetEnableSCPass() { return EnableSCPass; }

  /// SCOpts: Retrieve a map of StoneCutter pass options
  std::map<std::string,std::string> GetSCPassOptions() { return SCPassOpts; }

  /// SCOpts: Retrieve a map of StoneCutter Pipeline pass options
  std::map<std::string,std::string> GetPipelinePassOptions() { return PipePassOpts; }

  /// SCOpts: Retrieve a specific mapped option from the Pipeline pass map
  bool GetPipelinePassOption(const std::string Option, std::string &Value);

  /// SCOpts: Retrieve the output file name
  std::string GetOutputFile() { return OutFile; }

  /// SCOpts: Retrieve the signal map file name
  std::string GetSignalMapFile() { return SigMap; }

  /// SCOpts: Get the number of input files
  unsigned GetNumInputFiles() { return FileList.size(); }

  /// SCOpts: Get the target input file
  std::string GetInputFile( unsigned );

  /// SCOpts: Get the package name
  std::string GetPackage(){ return Package; }

  /// SCOpts: Get the ISA name
  std::string GetISA(){ return ISA; }

  /// SCOpts: Set the package name
  bool SetPackage( std::string P ){ Package = P; return true; }

  /// SCOpts: Set the ISA name
  bool SetISA( std::string I ){ ISA = I; return true; }

  /// SCOpts: Set the output file name
  bool SetOutputFile( std::string O ){ OutFile = O; return true; }

  /// SCOpts: Determines whether an output file name has been set
  bool IsOutputFile() { if( OutFile.length() > 0 ){ return true;} return false; }

  /// SCOpts: Set the input file name
  bool SetInputFile( std::string I );

  /// SCOpts: Purge the input file names
  bool PurgeInputFiles();

  /// SCOpts: Set the 'performance stats' option
  bool SetPerf() { isPerf = true; return true; }

  /// SCOpts: Set the 'area' opt
  bool SetAreaOpt() { isAreaOpt = true; isPowerOpt = false; isPerfOpt = false; return true; }

  /// SCOpts: Set the 'power' opt
  bool SetPowerOpt() { isPowerOpt = true; isAreaOpt = false; isPerfOpt = false; return true; }

  /// SCOpts: Set the 'performance' opt
  bool SetPerfOpt() { isPerfOpt = true; isPowerOpt = false; isAreaOpt = false; return true; }

  /// SCOpts: Set the 'chisel' option
  bool SetChisel() { isChisel = true; return true; }

  /// SCOpts: Set the 'keep' option
  bool SetKeep() { isKeep = true; return true; }

  /// SCOpts: Sets the 'parse' option
  bool SetParse() { isParse = true; return true; }

  /// SCOpts: Sets the 'ir' option
  bool SetIR() { isIR = true; return true; }

  /// SCOpts: Sets the 'optimize' option
  bool SetOptimize() { isOptimize = true; return true; }

  /// SCOpts: Sets thes 'codegen' option
  bool SetCG() { isCG = true; return true; }

  /// SCOpts: Sets the 'verbose' option
  bool SetVerbose() { isVerbose = true; return true; }

  /// SCOpts: Sets the 'sigmap' option
  bool SetSignalMap(std::string O) { isSigMap = true; SigMap = O; return true; }

  /// SCOpts: Enables the pipeliner
  bool SetPipeline() { isPipeline = true; return true; }

  /// SCOpts: Disables the 'chisel' option
  bool UnsetChisel() { isChisel = false; return false; }

  /// SCOpts: Disables the 'parse' option
  bool UnsetParse() { isParse = false; return true; }

  /// SCOpts: Disables the 'ir' option
  bool UnsetIR() { isIR = false; return true; }

  /// SCOpts: Disables the 'optimize' option
  bool UnsetOptimize() { isOptimize = false; return true; }

  /// SCOpts: Disables thes 'codegen' option
  bool UnsetCG() { isCG = false; return true; }

  /// SCOpts: Disables the 'verbose' option
  bool UnsetVerbose() { isVerbose = false; return true; }

  /// SCOpts: Disables the 'sigmap' option
  bool UnsetSignalMap() { isSigMap = false; return true; }

  /// SCOpts: Disables the 'keep' option
  bool UnsetKeep() { isKeep = false; return true; }

  /// SCOpts: Disables the pipeline option
  bool UnsetPipeline() { isPipeline = false; return true; }

  /// SCOpts: Disable SC passes
  bool UnsetSCPasses() { isSCEnable = true; EnableSCPass.clear(); return true; }

  /// SCOpts: Disables the performance stats
  bool UnsetPref() { isPerf = false; return true; }

  /// SCOpts: Disable Manual SC passes
  bool DisableManualSCPasses() { isSCEnable = false; return true; }
};

#endif // _STONECUTTER_SCOPTS_H_
