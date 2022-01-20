//
// _SCPerf_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCPerf
 *
 * \ingroup StoneCutter
 *
 * \brief StonCutter performance analysis class
 *
 */

#ifndef _STONECUTTER_SCPERF_H_
#define _STONECUTTER_SCPERF_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

class SCPerf{
private:
  std::ostream *mStream;                              ///< SCMsg: Stream handler
  std::vector<std::pair<std::string,unsigned>> UOps;  ///< SCPerf: vector of performance values

public:

  /// SCPerf: default constructor
  SCPerf();

  /// SCPerf: overriden constructor constructor
  SCPerf( std::ostream *Stream );

  /// SCPerf: default destructor
  ~SCPerf();

  /// SCPerf: Print the performance statistics
  void PrintStats();

  /// SCPerf: Inserts a new micro op for the target instruction
  bool InsertUOp( std::string Inst );

};

#endif

// EOF
