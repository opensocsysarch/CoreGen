//
// _SMPass_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SMPass
 *
 * \ingroup CoreGenSigMap
 *
 * \brief CoreGenSigMap analysis and optimization passes
 *
 */

#ifndef _COREGENSIGMAP_SMPASS_H_
#define _COREGENSIGMAP_SMPASS_H_

// standard headers
#include <string>
#include <vector>

// CoreGenSigmap headers
#include "CoreGen/CoreGenSigMap/SCSig.h"
#include "CoreGen/CoreGenSigMap/SCTmp.h"

class SMPass{
private:
  std::string Name;       ///< Name of the SigMap pass
  std::string Error;      ///< Error string


protected:
  std::vector<SCSig *> Signals;   ///< CoreGenSigMap: signal vector
  std::vector<SCTmp *> TempRegs;  ///< CoreGenSigMap: temporary register vector
  std::vector<SigType> TopSigs;   ///< CoreGenSigMap: top-level signals

public:
  /// Default constructor
  SMPass(std::string N,
         std::vector<SCSig *> SigVect,
         std::vector<SCTmp *> TempVect,
         std::vector<SigType> TopVect)
    : Name(N), Signals(SigVect), TempRegs(TempVect), TopSigs(TopVect) {}

  /// Default destructor
  virtual ~SMPass() {}

  /// Set the error string
  void SetErrorStr(std::string E) { Error = E; }

  /// Retrieve the name of the pass
  std::string GetName() { return Name; }

  /// Determines if there is an error present
  bool IsError() { if( Error.length() > 0 ){return true;}else{return false;}}

  /// Retrieves the error string
  std::string GetErrorStr() { return Error; }

  /// Executes the target signal map pass
  virtual bool Execute() = 0;
};

#endif

// EOF
