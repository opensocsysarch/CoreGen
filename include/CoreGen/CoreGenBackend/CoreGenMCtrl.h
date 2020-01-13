//
// _CoreGenMCtrl_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenMCtrl
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Memory Controller Nodes
 *
 */

#ifndef _COREGENMCTRL_H_
#define _COREGENMCTRL_H_

#include <iostream>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

typedef enum{
  CGWeak    = 0,            ///< CGMemOrder: Weak memory ordering
  CGTSO     = 1,            ///< CGMemOrder: Total store ordering
  CGStrong  = 2             ///< CGMemOrder: Strong memory ordering
}CGMemOrder;                ///< CoreGenMCtrl: Memory ordering types

class CoreGenMCtrl : public CoreGenNode{
private:
  unsigned InputPorts;        ///< CoreGenMCtrl: input ports
  CGMemOrder Order;           ///< CoreGenMCtrl: memory ordering

public:

  /// Default constructor
  CoreGenMCtrl(std::string Name, CoreGenErrno *Errno);

  /// Overloaded constructor
  CoreGenMCtrl(std::string Name, CoreGenErrno *Errno, unsigned Ports);

  /// Overloaded constructor
  CoreGenMCtrl(std::string Name, CoreGenErrno *Errno, unsigned Ports, CGMemOrder Order);

  /// Defeault destructor
  ~CoreGenMCtrl();

  /// Set the number of input ports
  bool SetNumInputPorts( unsigned P );

  /// Retrieve the number of input ports
  unsigned GetNumInputPorts();

  /// Retrieve the memory ordering
  CGMemOrder GetMemOrder() { return Order; }

  /// Set the memory ordering
  bool SetMemOrder( CGMemOrder O );
};

#endif

// EOF
