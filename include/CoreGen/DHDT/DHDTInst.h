//
// _DHDTInst_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class DHDTInst
 *
 * \ingroup DHDT
 *
 * \brief Deep Hardware Design Tool instruction class
 *
 */

#ifndef _DHDTINST_H_
#define _DHDTINST_H_

// C++ Headers
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>

// CoreGen Headers
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class DInst{
private:
  std::string Inst;       ///< DInst: instruction text
  unsigned BitLen;        ///< DInst: Bit length
  unsigned ByteLen;       ///< DInst: Byte length
  char *Buf;              ///< DInst: Buffer

public:
  /// DInst: Default constructor
  DInst(std::string Inst,unsigned BitLen)
    : Inst(Inst), BitLen(BitLen), Buf(nullptr) {
    if( BitLen%8 != 0 ){
      // adjust for non byte aligned payloads
      ByteLen = (BitLen/8)+1;
    }else{
      ByteLen = (BitLen/8);
    }
    Buf = new char[ByteLen];
    for( unsigned i=0; i<ByteLen; i++ ){
      Buf[i] = 0b00000000;
    }
  }

  /// DInst: Default destructor
  ~DInst(){if(Buf){delete Buf;}}

  /// DInst: Retrieve the bit length
  unsigned GetBitLen() { return BitLen; }

  /// DInst: Retrieve the byte length
  unsigned GetByteLen() { return ByteLen; }

  /// DInst: Write the target byte with the input integer
  bool WriteByte( unsigned TargetByte, uint8_t Byte ){
    if( TargetByte > (this->GetByteLen()-1) )
      return false;
    Buf[TargetByte] = Byte;
    return true;
  }

  /// DInst: Write the target 2-bytes with the input integer
  bool Write2Byte( unsigned TargetByte, uint16_t TwoByte ){
    if( TargetByte > (this->GetByteLen()-1) )
      return false;
    Buf[TargetByte] = (uint8_t)(TwoByte & 0b11111111);
    Buf[TargetByte+1] = (uint8_t)(TwoByte >> 8);
    return true;
  }
};

class DHDTInst{
private:
  bool isOpen;            ///< DHDTInst: Is the file open?
  unsigned LineNo;        ///< DHDTInst: Line number
  std::fstream IFile;     ///< DHDTInst: File stream

  CoreGenBackend &CG;     ///< DHDTInst: CoreGenBackend object

  std::vector<CoreGenInst *> Insts;         ///< DHDTInst: Vector of instruction objects
  std::vector<CoreGenVLIWStage *> Stages;   ///< DHDTInst: Vector of VLIW stages

  // todo: do we need a vector of VLIW stages?

  /// Determine if the current line is a comment
  bool IsComment(std::string Line);

  /// Build an instruction payload
  DInst *BuildInstPayload(std::string Inst);

  /// Build hex instruction payload
  DInst *BuildHexInstPayload(std::string Inst);

  /// Build binary instruction payload
  DInst *BuildBinaryInstPayload(std::string Inst);

  /// Build assembly instruction payload
  DInst *BuildAsmInstPayload(std::string Inst);

  /// Assemble a payload from an asm bundle
  DInst *AssemblePayload(CoreGenInst *Inst,
                         std::string AsmArgs,
                         std::string InstArgs);

  /// Cache the instruction nodes
  bool CacheInstNodes();

  /// Tokenizer for asm strings
  std::vector<std::string> GetAsmTokens(std::string Inst, char delim);

public:
  /// DHDTInst: Default constructor
  DHDTInst(std::string InstFile,
           CoreGenBackend &CG);

  /// DHDTInst: Default destructor
  ~DHDTInst();

  /// DHDTInst: Retrieve the current line number
  unsigned GetLineNumber() { return LineNo; }

  /// DHDTInst: Is the instruction file open?
  bool IsOpen() { return isOpen; }

  /// DHDTInst: Read the next instruction payload.  If nothing to read, return nullptr
  DInst *ReadInst();
};

#endif

// EOF
