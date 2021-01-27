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
  unsigned width;          ///< VarAttrs: width of the variable or register
  unsigned dimX;           ///< VarAttrs: number of elements in X dimension
  unsigned dimY;           ///< VarAttrs: number of elements in Y dimension
  int xIdx;                ///< VarAttrs: is the X index for a register within a vector/matrix element
  int yIdx;                ///< VarAttrs: is the Y index for a register within a vector/matrix element
  bool defSign;            ///< VarAtrrs: is the variable a signed integer
  bool defVector;          ///< VarAttrs: is the variable a vector (elems > 1)
  bool defMatrix;          ///< VarAttrs: is the variable a matrix (elems2D > 0)
  bool defFloat;           ///< VarAttrs: is the variable a floating point variable
  bool defElem;            ///< VarAttrs: is the variable in a vect/mat
  bool defRegClass;        ///< VarAttrs: does the variable represent a register class
  bool defReadOnly;        ///< VarAttrs: read-only register
  bool defReadWrite;       ///< VarAttrs: read-write register
  bool defCSR;             ///< VarAttrs: CSR register
  bool defAMS;             ///< VarAttrs: AMS register
  bool defTUS;             ///< VarAttrs: Thread unit shared register
  bool defShared;          ///< VarAttrs: Shared register
  std::string containers;  ///< VarAttrs: name of containing vect/matrix if applicable 
}VarAttrs;

/** VarAttrEntry struct: contains a single element for the VarAttrEntryTable */
typedef struct{
  std::string Name;       ///< VarAttrEntry: base name of the datatype
  unsigned width;         ///< VarAttrEntry: width of the type in bits
  unsigned dimX;          ///< VarAttrEntry: number of elements in the type's X-dimesion
  unsigned dimY;          ///< VarAttrEntry: number of elements in the type's Y-dimension
  bool IsDefSign;         ///< VarAttrEntry: is the type a signed integer
  bool IsDefFloat;        ///< VarAttrEntry: is the type a floating point
  bool IsDefVector;       ///< VarAttrEntry: is the type a vector (elems > 1)
  bool IsDefMatrix;       ///< VarAttrEntry: is the type a matrix (elems2D > 0)
  bool IsDefElem;         ///< VarAttrEntry: is this an element of a vector/matrix
  int xIdx;               ///< VarAttrEntry: is the X index for a register within a vector/matrix element
  int yIdx;               ///< VarAttrEntry: is the Y index for a register within a vector/matrix element
  std::vector<std::string> containers;  ///< VarAttrEntry: name of containing vectors/matrix if applicable 
}VarAttrEntry;

/** Contains a list of commonly found datatypes in StoneCutter */
const VarAttrEntry VarAttrEntryTable[] = {
  // NAME WIDTH DIMX DIMY ISDEFSIGN ISDEFFLOAT ISDEFVECTOR ISDEFMATRIX ISDEFELEM XIDX YIDX CONTAINER
  { "float",  32, 1, 0, false, true,  false, false, false, -1, -1},
  { "double", 64, 1, 0, false, true,  false, false, false, -1, -1},
  { "bool",    1, 1, 0, false, false, false, false, false, -1, -1},
  { "u8",      8, 1, 0, false, false, false, false, false, -1, -1},
  { "u16",    16, 1, 0, false, false, false, false, false, -1, -1},
  { "u32",    32, 1, 0, false, false, false, false, false, -1, -1},
  { "u64",    64, 1, 0, false, false, false, false, false, -1, -1},
  { "s8",      8, 1, 0, true,  false, false, false, false, -1, -1},
  { "s16",    16, 1, 0, true,  false, false, false, false, -1, -1},
  { "s32",    32, 1, 0, true,  false, false, false, false, -1, -1},
  { "s64",    32, 1, 0, true,  false, false, false, false, -1, -1},
  { ".",       0, 0, 0, false, false, false, false, false, -1, -1}  // disable flag
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
