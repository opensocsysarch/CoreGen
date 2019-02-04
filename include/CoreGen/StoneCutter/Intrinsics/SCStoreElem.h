//
// _SCStoreElem_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCStoreElem
 *
 * \ingroup StoneCutter
 *
 * \brief StoneCutter Intrinsic: Store a Value To Memory Using the Target Element Size
 *        STORE(VALUE,TARGET,SIZE)
 *
 */

#ifndef _STONECUTTER_SCINTRIN_SCSTOREELEM_H_
#define _STONECUTTER_SCINTRIN_SCSTOREELEM_H_

#include "CoreGen/StoneCutter/SCIntrin.h"

class SCStoreElem : public SCIntrin {
private:
public:
  /// Default cosntructor
  SCStoreElem();

  /// Overloaded Constructor
  SCStoreElem( unsigned NI, std::string K );

  /// Default destructor
  ~SCStoreElem();

  /// Execute the codegen
  virtual Value *codegen();
};

#endif

// EOF
