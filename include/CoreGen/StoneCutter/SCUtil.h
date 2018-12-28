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

// standard headers
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

// llvm headers
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/GlobalVariable.h"

using namespace llvm;

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

// CoreGenUtil: Retrieve the current date+time
inline std::string SCCurrentDateTime(){
  time_t now = time(0);
  std::string DT(ctime(&now));
  return DT;
}

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

inline Type *GetLLVarType( VarAttrs VA, LLVMContext TC ){
  if( (VA.defFloat) && (VA.width==32) ){
    return Type::getFloatTy(TC);
  }else if( (VA.defFloat) && (VA.width==64) ){
    return Type::getDoubleTy(TC);
  }else{
    return Type::getIntNTy(TC,VA.width);
  }
}

#endif

// EOF
