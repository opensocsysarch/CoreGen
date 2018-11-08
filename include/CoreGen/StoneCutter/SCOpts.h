//
// _SCOpts_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCOPTS_H_
#define _STONECUTTER_SCOPTS_H_

#include <iostream>
#include <string>
#include <vector>
#include <streambuf>

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

  std::string OutFile;  ///< SCOpts: Output file designator

  std::vector<std::string> FileList;  ///< SCOpts: List of files to compile

  SCMsg *Msgs;    ///< SCOpts: Message handlers

  // private functions
  /// Print the help menu
  void PrintHelp();

  /// Print the version information
  void PrintVersion();

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

  /// SCOpts: Do we execute the optimizer
  bool IsOptimize() { return isOptimize; }

  /// SCOpts: Is verbosity enabled?
  bool IsVerbose() { return isVerbose; }

  /// SCOpts: Retrieve the output file name
  std::string GetOutputFile() { return OutFile; }

  /// SCOpts: Get the number of input files
  unsigned GetNumInputFiles() { return FileList.size(); }

  /// SCOpts: Get the target input file
  std::string GetInputFile( unsigned );

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
};

#endif // _STONECUTTER_SCOPTS_H_
