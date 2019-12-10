//
// _SCMsg_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCMsg
 *
 * \ingroup StoneCutter
 *
 * \brief StonCutter message printing class
 *
 */

#ifndef _STONECUTTER_SCMSG_H_
#define _STONECUTTER_SCMSG_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

typedef enum{
  L_FATAL,    /*! SCMsg: Fatal messages */
  L_WARN,     /*! SCMsg: Warning messages */
  L_ERROR,    /*! SCMsg: Error messages */
  L_DEBUG,    /*! SCMsg: Debug messages */
  L_MSG       /*! SCMsg: Standard message */
}MSG_LEVEL;

class SCMsg{
private:
  // private variables
  bool isCout;            /*! SCMsg: Determines whether COUT is used */
  std::ostream *mStream;  /*! SCMsg: Stream handler */

  // private functions
  void doExit();    /*! SCMsg: Abort */

public:
  /// SCMsg: Constructor
  SCMsg();

  /// SCMsg: Overridden constructor
  SCMsg( std::streambuf* sb );

  /// SCMsg: Destructor
  ~SCMsg();

  /// SCMsg: Set the stream handler
  bool SetStream( std::streambuf* sb );

  /// SCMsg: Print a raw message to the output stream
  void PrintRawMsg( const std::string S );

  /// SCMsg: Print a message
  void PrintMsg( MSG_LEVEL L, const std::string M );

  /// SCMsg: Print a debug message
  void PrintDebugMsg( const char *F,
                      int Line,
                      const std::string M );

  /// SCMsg: Sets the message stream to STDOUT
  bool SetStdout();

  /// SCMSg: Retrieve the output stream
  std::ostream *GetStream() { return mStream; }
};

#endif // _STONECUTTER_SCMSG_H_
