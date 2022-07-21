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

class DInstMask{
private:
  CoreGenInst *Inst;                    ///< DInstMask: Target instruction
  CoreGenInstFormat *Format;            ///< DInstMask: Instruction format
  std::vector<CoreGenEncoding *> Encs;  ///< DInstMask: Encodings

public:
  /// DInstMask: Default constructor
  DInstMask(CoreGenInst *I,
            CoreGenInstFormat *F)
    : Inst(I), Format(F) { }

  /// DInstMask: Default destructor
  ~DInstMask();

  /// DInstMask: Add an encoding
  void AddEncoding(CoreGenEncoding *E) { Encs.push_back(E); }

  /// DInstMask: Retrieve the instruction pointer
  CoreGenInst *GetInst() { return Inst; }

  /// DInstMask: Retrieve the instruction format
  CoreGenInstFormat *GetFormat() { return Format; }

  /// DInstMask: Retrieve the number of encodings
  unsigned GetNumEncodings() { return Encs.size(); }

  /// DInstMask: Retrieve the target encoding
  CoreGenEncoding *GetEncoding(unsigned E){
    if( E > (Encs.size()-1) )
      return nullptr;
    return Encs[E];
  }
};

class DInst{
private:
  std::string Inst;       ///< DInst: instruction text
  unsigned BitLen;        ///< DInst: Bit length
  unsigned ByteLen;       ///< DInst: Byte length
  char *Buf;              ///< DInst: Buffer

  CoreGenInst *IPtr;      ///< DInst: Instruction payload pointer

public:
  /// DInst: Default constructor
  DInst(std::string Inst,unsigned BitLen)
    : Inst(Inst), BitLen(BitLen), Buf(nullptr), IPtr(nullptr) {
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

  /// DInst: Overloaded constructor
  DInst(std::string Inst, CoreGenInst *I)
    : Inst(Inst), BitLen(I->GetFormat()->GetFormatWidth()),
      Buf(nullptr), IPtr(I) {
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

  /// DInst: Retrieve the text of the instruction
  std::string GetStr() { return Inst; }

  /// DInst: Retrieve the instruction pointer
  CoreGenInst *GetInstPtr() { return IPtr; }

  /// DInst: Retrieve the bit length
  unsigned GetBitLen() { return BitLen; }

  /// DInst: Retrieve the byte length
  unsigned GetByteLen() { return ByteLen; }

  /// DInst: Extract the target bits
  uint64_t ExtractBits( unsigned StartBit, unsigned EndBit ){
    unsigned CurElem = StartBit/8;
    unsigned CurBit  = StartBit%8;
    uint64_t bits = 0x00ull;
    unsigned shift = 0;

    for( unsigned i=StartBit; i<EndBit; i++ ){

      if( (Buf[CurElem] & (1<<CurBit)) ){
        bits |= (1<<shift);
      }

      // increment the bit
      CurBit++;
      if( CurBit == 8 ){
        CurElem++;
        CurBit = 0;
      }
      // increment the shift
      shift++;
    }

    return bits;
  }

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
  std::vector<DInstMask *> Masks;           ///< DHDTInst: Vector of instruction masks
  std::vector<CoreGenVLIWStage *> Stages;   ///< DHDTInst: Vector of vliw stage objects

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

  /// Cache the instruction nodes
  bool CacheInstNodes();

  /// Cache the VLIW stage nodes
  bool CacheVLIWNodes();

  /// Build an instruction mask
  bool BuildInstMask(CoreGenInst *Inst);

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

  /// DHDTInst: Crack the target instruction payload.  If crack fails, return empty string
  std::string CrackInst(DInst *Inst);
};

#endif

// EOF
