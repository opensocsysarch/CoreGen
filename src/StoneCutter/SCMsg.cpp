//
// _SCMsg_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCMsg.h"

// ------------------------------------------------- CONSTRUCTOR
SCMsg::SCMsg()
  : isCout(true), mStream(NULL){
  mStream = &std::cout;
}

// ------------------------------------------------- DESTRUCTOR
SCMsg::~SCMsg(){
  if( (!isCout) && (mStream) ){
    delete mStream;
  }
}

// ------------------------------------------------- DOEXIT
void SCMsg::doExit(){
  std::abort();
}

// ------------------------------------------------- SETSTDOUT
bool SCMsg::SetStdout(){
  if( isCout ){
    return true;
  }

  if( mStream ){
    delete mStream;
  }

  mStream = &std::cout;
  isCout = true;

  return true;
}

// ------------------------------------------------- SETSTREAM
bool SCMsg::SetStream( std::streambuf* sb ){
  if( !sb ){
    return false;
  }

  if( isCout ){
    mStream = NULL;
    isCout = false;
  }

  mStream = new std::ostream(sb);

  return true;
}

// ------------------------------------------------- PRINTRAWMSG
void SCMsg::PrintRawMsg( const std::string M ){
  *mStream << M << std::endl;
}

// ------------------------------------------------- PRINTDEBUGMSG
void SCMsg::PrintDebugMsg( const char *F,
                           int Line,
                           const std::string M ){
}

// ------------------------------------------------- PRINTMSG
void SCMsg::PrintMsg( MSG_LEVEL L, const std::string M ){
  switch( L ){
  case L_FATAL:
    *mStream << "FATAL ERROR : " << M << std::endl;
    doExit();
    break;
  case L_WARN:
    *mStream << "WARNING : " << M << std::endl;
    break;
  case L_ERROR:
    *mStream << "ERROR : " << M << std::endl;
    break;
  case L_DEBUG:
    *mStream << "DEBUG  : " << M << std::endl;
    break;
  case L_MSG:
    *mStream << M << std::endl;
    break;
  default:
    *mStream << "UNKNOWN : " << M << std::endl;
    break;
  }
}

// EOF
