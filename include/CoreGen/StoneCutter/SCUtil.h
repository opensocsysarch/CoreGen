//
// _SCUtil_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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

/** VarAttrs struct: contains the potential parameters for variable attributes */
typedef struct{
  unsigned width;   ///< VarAttrs: width of the variable or register
  unsigned elems;   ///< VarAttrs: number of elements in the variable
  bool defSign;     ///< VarAtrrs: is the variable a signed integer
  bool defVector;   ///< VarAttrs: is the variable a vector (elems > 1)
  bool defFloat;    ///< VarAttrs: is the variable a floating point variable
  bool defRegClass; ///< VarAttrs: does the variable represent a register class
  bool defReadOnly; ///< VarAttrs: read-only register
  bool defReadWrite;///< VarAttrs: read-write register
  bool defCSR;      ///< VarAttrs: CSR register
  bool defAMS;      ///< VarAttrs: AMS register
  bool defTUS;      ///< VarAttrs: Thread unit shared register
  bool defShared;   ///< VarAttrs: Shared register
}VarAttrs;

/** VarAttrEntry struct: contains a single element for the VarAttrEntryTable */
typedef struct{
  std::string Name;   ///< VarAttrEntry: base name of the datatype
  unsigned width;     ///< VarAttrEntry: width of the type in bits
  unsigned elems;     ///< VarAttrEntry: number of elements in the type
  bool IsDefSign;     ///< VarAttrEntry: is the type a signed integer
  bool IsDefVector;   ///< VarAttrEntry: is the type a vector (elems > 1)
  bool IsDefFloat;    ///< VarAttrEntry: is the type a floating point
}VarAttrEntry;

/** Contains a list of commonly found datatypes in StoneCutter */
const VarAttrEntry VarAttrEntryTable[] = {
  { "float",  32, 1, false, false, true },
  { "double", 64, 1, false, false, true },
  { "bool",    1, 1, false, false, false },
  { "u8",      8, 1, false, false, false },
  { "u16",    16, 1, false, false, false },
  { "u32",    32, 1, false, false, false },
  { "u64",    64, 1, false, false, false },
  { "s8",      8, 1, true,  false, false },
  { "s16",    16, 1, true,  false, false },
  { "s32",    32, 1, true,  false, false },
  { "s64",    32, 1, true,  false, false },
  { ".",       0, 0, false, false, false }  // disable flag
};

/** Contains a list of pipeline attributes */
const std::string PipeAttrTable[] = {
  "in_order",
  "out_of_order",
  "branch_predict",
  "forward",
  "." // disable flag
};

// SCUtil: Retrieve the current date+time
inline std::string SCCurrentDateTime(){
  time_t now = time(0);
  std::string DT(ctime(&now));
  return DT;
}

// SCUtil: Does the target file already exist
inline bool SCFileExists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

// SCUtil: Delete the target file
inline bool SCDeleteFile(const std::string& name){
  if( !SCFileExists(name) )
    return true;
  if( remove(name.c_str()) != 0 ){
    return false;
  }else{
    return true;
  }
}

// SCUtil: Retrieve the LLVM variable type
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
