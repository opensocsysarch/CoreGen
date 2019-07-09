//
// _SCOpts_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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
#define SC_PATCH_VERSION 0
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
  bool isEnable;    ///< SCOpts: Manually disabled passes flag
  bool isListPass;  ///< SCOpts: User selected pass listing flag
  bool isSigMap;    ///< SCOpts: Signal map selected

  std::string OutFile;  ///< SCOpts: Output file designator
  std::string SigMap;   ///< SCOpts: Signal map output file

  std::string Package;  ///< SCOpts: Chisel package name
  std::string ISA;      ///< SCOpts: Chisel ISA name

  std::vector<std::string> FileList;  ///< SCOpts: List of files to compile

  std::vector<std::string> EnablePass;  ///< SCOpts: List of enabled passes
  std::vector<std::string> DisablePass; ///< SCOpts: List of disabled passes

  SCMsg *Msgs;    ///< SCOpts: Message handlers

  // private functions
  /// Print the help menu
  void PrintHelp();

  /// Print the version information
  void PrintVersion();

  /// Parse the pass options and split them into a vector
  std::vector<std::string> ParsePasses(std::string P);

  /// Split the options list by commas
  void Split(const std::string &s, char delim,
             std::vector<std::string>& v);

  /// Derives the ISA name from the path
  std::string GetISANameFromPath();

  /// Removes any dots in the ISA name
  std::string SCRemoveDot(std::string);

public:
  /// SCOpts: Constructor
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

  /// SCOpts: Do we execute the parser
  bool IsParse() { return isParse; }

  /// SCOpts: Do we have manually disabled passes
  bool IsDisablePass() { return isDisable; }

  /// SCOpts: Do we have manually enabled passes
  bool IsEnablePass() { return isEnable; }

  /// SCOpts: Do we need to list the passes
  bool IsListPass() { return isListPass; }

  /// SCOpts: Do we execute the optimizer
  bool IsOptimize() { return isOptimize; }

  /// SCOpts: Is verbosity enabled?
  bool IsVerbose() { return isVerbose; }

  /// SCOpts: Is signal map enabled?
  bool IsSignalMap() { return isSigMap; }

  /// SCOpts: Retrieve the list of disabled passes
  std::vector<std::string> GetDisabledPass() { return DisablePass; }

  /// SCOpts: Rerieve the list of enabled passes
  std::vector<std::string> GetEnablePass() { return EnablePass; }

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
};

#endif // _STONECUTTER_SCOPTS_H_
