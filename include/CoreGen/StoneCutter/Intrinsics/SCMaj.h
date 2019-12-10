//
// _SCMaj_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCMaj
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Three Input Majority Vote
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCMAJ_H_
#define _STONECUTTER_SCINTRIN_SCMAJ_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCMaj : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCMaj();

  /// Overloaded Constructor
  SCMaj( unsigned NI, std::string K );

  /// Default destructor
  ~SCMaj();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
