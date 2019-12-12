//
// _CoreGenErrno_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenErrno
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Error Handlers
 *
 */

#ifndef _COREGENERRNO_H_
#define _COREGENERRNO_H_

#include <iostream>
#include <string>
#include <cerrno>

typedef enum{
  CGERR_NONE    = 0,  ///< CGERR: No Error/Message
  CGERR_STATUS  = 1,  ///< CGERR: Status Message
  CGERR_ERROR   = 2,  ///< CGERR: Error
  CGERR_WARN    = 3,  ///< CGERR: Warning
  CGERR_FATAL   = 10  ///< CGERR: Fatal Error
}CGERR;               ///< CoreGenErrno: Error Types

class CoreGenErrno
{
private:
  CGERR ErrCode;      ///< CoreGenErrno: Error code
  std::string ErrStr; ///< CoreGenErrno: Error string

public:
  /// Default Constructor
  CoreGenErrno();

  /// Default Destructor
  ~CoreGenErrno();

  /// Set an error or message state
  bool SetError( CGERR Code, std::string Str );

  /// Retrieve the error or message state and message
  bool GetError( CGERR *Code, std::string *Str );

  /// Is the current error state fatal?
  bool IsFatal();

  /// Retrieve the current error or message state
  std::string GetErrStr();
};

#endif

// EOF
