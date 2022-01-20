//
// _CoreGenSpad_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenSpad
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Scratchpad Memory Nodes
 *
 */

#ifndef _COREGENSPAD_H_
#define _COREGENSPAD_H_

#include <iostream>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

class CoreGenSpad : public CoreGenNode{
private:
  unsigned MemSize;       ///< CoreGenSpad: memory size
  unsigned RqstPorts;     ///< CoreGenSpad: request ports
  unsigned RspPorts;      ///< CoreGenSpad: response ports
  uint64_t StartAddr;     ///< CoreGenSpad: starting address

public:

  /// Default constructor
  CoreGenSpad(std::string Name, CoreGenErrno *Errno);

  /// Overloaded constructor
  CoreGenSpad(std::string Name, CoreGenErrno *Errno,
              unsigned MSize, unsigned RqPorts, unsigned RsPorts );

  /// Defeault destructor
  ~CoreGenSpad();

  /// Set the scratchpad memory size
  bool SetMemSize( unsigned M );

  /// Set the number of request ports
  bool SetRqstPorts( unsigned P );

  /// Set the number of response ports
  bool SetRspPorts( unsigned P );

  /// Set the start address
  bool SetStartAddr( uint64_t Addr );

  /// Retrieve the memory size
  unsigned GetMemSize();

  /// Retrieve the number of memory request ports
  unsigned GetRqstPorts();

  /// Retrieve the number of memory response ports
  unsigned GetRspPorts();

  /// Retrieve the starting address of the scratchpad
  uint64_t GetStartAddr();
};

#endif

// EOF
