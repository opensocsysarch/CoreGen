//
// _DHDTOpts_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class DHDTOpts
 *
 * \ingroup DHDT
 *
 * \brief Deep Hardware Design Tool options class
 *
 */

#ifndef _DHDTOPTS_H_
#define _DHDTOPTS_H_

#include <iostream>
#include <string>

#ifndef DHDT_MAJOR_VERSION
#define DHDT_MAJOR_VERSION 0
#endif

#ifndef DHDT_MINOR_VERSION
#define DHDT_MINOR_VERSION 1
#endif

#ifndef DHDT_PATCH_VERSION
#define DHDT_PATCH_VERSION 0
#endif

class DHDTOpts{
private:
  // private variables
  int argc;                 ///< DHDTOpts: ARGC command line info
  char **argv;              ///< DHDTOpts: ARGV command line info

  bool isHazard;            ///< DHDTOpts: Is hazard detection enabled
  bool isPower;             ///< DHDTOpts: Is power estimation enabled

  std::string IRFile;       ///< DHDTOpts: LLVM IR File
  std::string CFile;        ///< DHDTOpts: CoreGen IR File
  std::string OutFile;      ///< DHDTOpts: Output File
  std::string PFile;        ///< DHDTOpts: Power file

  // private functions
  /// DHDTOpts: Print the help menu
  void PrintHelp();

  /// DHDTOpts: Print the version info
  void PrintVersion();

public:
  /// DHDTOpts: Constructor
  DHDTOpts(int,char **);

  /// DHDTOpts: Destructor
  ~DHDTOpts();

  /// DHDTOpts: Parse the command line options
  bool ParseOpts(bool &isHelp);

  /// DHDTOpts: Is the hazard detection enabled?
  bool IsHazard() { return isHazard; }

  /// DHDTOpts: Is the power estimation enabled?
  bool IsPower() { return isPower; }

  /// DHDTOpts: Retrieve the LLVM IR file name
  std::string GetLLVMIRFile() { return IRFile; }

  /// DHDTOpts: Retrieve the CoreGen IR file name
  std::string GetCoreGenIRFile() { return CFile; }

  /// DHDTOpts: Retrieve the power configuration file
  std::string GetPowerFile() { return PFile; }

  /// DHDTOpts: Retrieve the output file name
  std::string GetOutFile() { return OutFile; }
};

#endif

// EOF
