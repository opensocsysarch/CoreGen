//
// _CoreGenMCtrl_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

class CoreGenMCtrl : public CoreGenNode{
private:
  unsigned InputPorts;        ///< CoreGenMCtrl: input ports

public:

  /// Default constructor
  CoreGenMCtrl(std::string Name, CoreGenErrno *Errno);

  /// Overloaded constructor
  CoreGenMCtrl(std::string Name, CoreGenErrno *Errno, unsigned Ports);

  /// Defeault destructor
  ~CoreGenMCtrl();

  /// Set the number of input ports
  bool SetNumInputPorts( unsigned P );

  /// Retrieve the number of input ports
  unsigned GetNumInputPorts();
};

#endif

// EOF
