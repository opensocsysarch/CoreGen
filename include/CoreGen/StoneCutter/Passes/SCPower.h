//
// _SCPass_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCPass
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter CodeGen Pass: Constructs the arithmetic portions of the Chisel pipeline
 *
 */

#ifndef _STONECUTTER_SCPASS_SCPOWER_H_
#define _STONECUTTER_SCPASS_SCPOWER_H_

#include <map>
#include <vector>
#include <tuple>
#include "CoreGen/StoneCutter/SCPass.h"
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

class SCPower : public SCPass {
  private: 
    struct InstInfo {
      std::string Name;
      unsigned Cycles;
    } InstInfo;

    void GetActiveCycles();

  public:
    SCPower(Module *TM, SCOpts *O, SCMsg *M);

    ~SCPower();

    virtual bool Execute() override;

};

#endif

// EOF