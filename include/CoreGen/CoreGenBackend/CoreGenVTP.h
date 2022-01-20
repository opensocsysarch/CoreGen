//
// _CoreGenVTP_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenVTP
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Virtual to Physical Node Handler
 *
 */

#ifndef _COREGENVTP_H_
#define _COREGENVTP_H_

#include <iostream>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

class CoreGenVTP : public CoreGenNode{
private:
public:

    /// Default constructor
    CoreGenVTP(std::string Name, CoreGenErrno *Errno);

    /// Defeault destructor
    ~CoreGenVTP();
};

#endif

// EOF
