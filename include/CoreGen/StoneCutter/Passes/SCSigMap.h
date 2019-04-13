//
// _SCSigMap_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCSigMap
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Generates a signal map for the target ISA
 *
 */

#ifndef _STONECUTTER_SCPASS_SCSIGMAP_H_
#define _STONECUTTER_SCPASS_SCSIGMAP_H_

#include <map>
#include <vector>
#include "CoreGen/StoneCutter/SCPass.h"

class SCSigMap : public SCPass {
private:
  std::string SigMap;         ///< Signal map output file

public:
  /// Default cosntructor
  SCSigMap(Module *TM, SCOpts *O, SCMsg *M);

  /// Default destructor
  ~SCSigMap();

  /// Execute the codegen
  virtual bool Execute() override;

  /// Sets the signal map file name
  bool SetSignalMapFile(std::string SM);
};

#endif

// EOF
