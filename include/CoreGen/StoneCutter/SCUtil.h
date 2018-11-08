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

typedef struct{
  unsigned width;
  unsigned elems;
  bool defSign;
  bool defVector;
  bool defFloat;
}VarAttrs;

typedef struct{
  std::string Name;
  unsigned width;
  unsigned elems;
  bool IsDefSign;
  bool IsDefVector;
  bool IsDefFloat;
}VarAttrEntry;

const VarAttrEntry VarAttrEntryTable[] = {
  { "float",  32, 1, false, false, true },
  { "double", 64, 1, false, false, true },
  { "bool",    1, 1, false, false, false },
  { "u8",      8, 1, false, false, false },
  { "u16",    16, 1, false, false, false },
  { "u32",    32, 1, false, false, false },
  { "u64",    32, 1, false, false, false },
  { "s8",      8, 1, true,  false, false },
  { "s16",    16, 1, true,  false, false },
  { "s32",    32, 1, true,  false, false },
  { "s64",    32, 1, true,  false, false },
  { ".",       0, 0, false, false, false }  // disable flag
};

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
