//
// _CoreGenComm_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenComm
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Communication Channel Handler
 *
 */

#ifndef _COREGENCOMM_H_
#define _COREGENCOMM_H_

#include <iostream>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

typedef enum{
  CGCommP2P = 0,      ///< CGCommType: Point to point
  CGCommBus = 1,      ///< CGCommType: Bus
  CGCommNoc = 2,      ///< CGCommType: Network on Chip
  CGCommUnk = 100     ///< CGCommType: Unknown (user-defined)
}CGCommType;

class CoreGenComm : public CoreGenNode{
private:
    CGCommType Type;    ///< CoreGenComm: Communication channel type
    unsigned Width;     ///< CoreGenComm: Communication channel width (in bits)
    std::vector<CoreGenNode *> EndPoints; ///< CoreGenComm: Communication endpoint nodes
public:

    /// Default constructor
    CoreGenComm(std::string Name, CoreGenErrno *Errno);

    /// Overloaded constructor
    CoreGenComm(std::string Name, CGCommType Type,
                unsigned Width, CoreGenErrno *Errno );

    /// Defeault destructor
    ~CoreGenComm();

    /// Insert an endpoint into the comm
    bool InsertEndpoint( CoreGenNode *Node );

    /// Delete an endpoint from the comm
    bool DeleteEndpoint( CoreGenNode *Node );

    /// Retrieve the type of the communication channel
    CGCommType GetCommType() { return Type; }

    /// Set the communication channel type
    bool SetCommType( CGCommType T );

    /// Retrieve the width of the communication channel
    unsigned GetWidth() { return Width; }

    /// Set the width of the communication channel
    bool SetWidth( unsigned W );

    /// Retrieve the endpoints
    std::vector<CoreGenNode *> GetEndpoints() { return EndPoints; }

    /// Retrieve the number of endpoints
    unsigned GetNumEndpoints() { return EndPoints.size(); }

    /// Retrieve the target endpoint
    CoreGenNode *GetEndpoint(unsigned Idx);

    /// Retrieve the target endpoint
    CoreGenNode *GetEndpoint( std::string Name );
};

#endif

// EOF
