//
// _SCUtil_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _STONECUTTER_SCUTIL_H_
#define _STONECUTTER_SCUTIL_H_

#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

inline bool SCFileExists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

inline bool SCDeleteFile(const std::string& name){
  if( remove(name.c_str()) != 0 ){
    return false;
  }else{
    return true;
  }
}

#endif

// EOF
