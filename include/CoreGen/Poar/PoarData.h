//
// _PoarData_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class PoarData
 *
 * \ingroup Poar
 *
 * \brief Power/Area speculation data class
 *
 */

#ifndef _POARDATA_H_
#define _POARDATA_H_

//-- C++ headers
#include <string>
#include <vector>

//-- CoreGen headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

//-- Poar headers
#include "CoreGen/Poar/PoarOpts.h"
#include "CoreGen/Poar/PoarConfig.h"
#include "CoreGen/Poar/PoarIO.h"

class PoarData{
public:
  /// PoarData: default constructor
  PoarData(PoarOpts *Opts);

  /// PoarData: default destructor
  ~PoarData();

  /// PoarData: retrieve the error string
  std::string GetErrStr() { return ErrStr; }

  /// PoarData: initialize the object
  bool Init();

  /// PoarData: derive the power and area data
  bool DeriveData();

  /// PoarData: write the data out to a file
  bool WriteData();

private:
  PoarOpts *POpts;          ///< PoarData: Poar options handler object
  PoarConfig *PConfig;      ///< PoarData: Poar configuration entry
  CoreGenBackend *CG;       ///< PoarData: CoreGenBackend object
  CoreGenSigMap *SM;        ///< PoarData: Signal Map object

  std::string ErrStr;       ///< PoarData: error string
};

#endif

// EOF
