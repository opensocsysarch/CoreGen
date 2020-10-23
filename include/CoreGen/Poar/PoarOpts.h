//
// _PoarOpts_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarOpts
 *
 * \ingroup Poar
 *
 * \brief Power/Area speculation options class
 *
 */

#ifndef _POAROPTS_H_
#define _POAROPTS_H_

#include <iostream>
#include <string>

#ifndef POAR_MAJOR_VERSION
#define POAR_MAJOR_VERSION 0
#endif

#ifndef POAR_MINOR_VERSION
#define POAR_MINOR_VERSION 1
#endif

#ifndef POAR_PATCH_VERSION
#define POAR_PATCH_VERSION 1
#endif

class PoarOpts{
private:
  // private variables
  int argc;           ///< PoarOpts: ARGC command line info
  char **argv;        ///< PoarOpts: ARGV command line info

  // private functions

  /// PoarOpts: Print the help menu
  void PrintHelp();

  /// PoarOpts: Print the version info
  void PrintVersion();

public:
  /// PoarOpts: Constructor
  PoarOpts(int,char **);

  /// PoarOpts: Destructor
  ~PoarOpts();

  /// PoarOpts: Parse the command line options
  bool ParseOpts(bool *isHelp);
};

#endif // _POAROPTS_H_

