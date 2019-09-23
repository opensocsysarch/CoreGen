//
// _CoreGenReg_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//


/**
 * \class CoreGenReg
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Register Node Handler
 *
 */


#ifndef _COREGENREG_H_
#define _COREGENREG_H_

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

class CoreGenReg : public CoreGenNode{
private:
  std::string pName;      ///< CoreGenReg: Register PseudoName

  bool isFixedValue;      ///< CoreGenReg: Is the register value fixed?
  bool isSIMD;            ///< CoreGenReg: Is this a SIMD register
  bool isIdxSet;          ///< CoreGenReg: Is the register index set?
  bool isShared;          ///< CoreGenReg: Is the register file shared across cores?

  int index;              ///< CoreGenReg: Register Index
  int width;              ///< CoreGenReg: Register width
  int SIMDwidth;          ///< CoreGenReg: SIMD width

  uint32_t attrs;         ///< CoreGenReg: Register Attributes

  std::vector<uint64_t> fixedVals;    ///< CoreGenReg: Fixed Value Vector
  std::vector<std::tuple< std::string,  // Subregister name
                          unsigned,     // Subregister start bit
                          unsigned>     // Subregister end bit
                            > SubRegs; ///< CoreGenReg: Subregister encodings

public:
  /// CoreGenReg: Register Attributes
  typedef enum{
    CGRegRO   = 0x01,     ///< CGRegAttr: Read-Only Register
    CGRegRW   = 0x02,     ///< CGRegAttr: Read-Write Register
    CGRegCSR  = 0x04,     ///< CGRegAttr: CSR
    CGRegAMS  = 0x08,     ///< CGRegAttr: Arithmetic Machine State
    CGRegTUS  = 0x10,     ///< CGRegAttr: Thread unit shared
    CGRegPC   = 0x20      ///< CGRegAttr: PC register (one per reg file)
  }CGRegAttr;             ///< CoreGenReg: Register attributes

  /// Default Constructor
  CoreGenReg(std::string RegName, int RegIndex,
             int RegWidth, CoreGenErrno *Errno );

  /// Is the register a fixed value?
  bool IsFixedValue() { return isFixedValue; }

  /// Is the register a SIMD register?
  bool IsSIMD() { return isSIMD; }

  /// Is the register a RW register?
  bool IsRWAttr();

  /// Is the register a RO register?
  bool IsROAttr();

  /// Is the register a CSR register?
  bool IsCSRAttr();

  /// Is the register an AMS register?
  bool IsAMSAttr();

  /// Is the register shared across thread units?
  bool IsTUSAttr();

  /// Is the register a PC register?
  bool IsPCAttr();

  /// Is the register file shared?
  bool IsShared() { return isShared; }

  /// Is the register index set?
  bool IsIndexSet() { return isIdxSet; }

  /// Retrieve the register index
  int GetIndex() { return index; }

  /// Retrieve the register width
  int GetWidth() { return width; }

  /// Retrieve the SIMD width
  int GetSIMDWidth() { if( isSIMD ) { return SIMDwidth; }else{ return 1;} }

  /// Retrieve the fixed register values
  bool GetFixedVals( std::vector<uint64_t> &FixedVals );

  /// Set the fixed register values
  bool SetFixedVals( std::vector<uint64_t> FixedVals );

  /// Set the fixed register value for a SIMD index
  bool SetFixedVals( uint64_t FixedVal, unsigned Idx );

  /// Retrieve the register attributes
  uint32_t GetAttrs() { return attrs; }

  /// Retrieve the number of subregisters
  unsigned GetNumSubRegs() { return SubRegs.size(); }

  /// Retrieve the target subregister data
  bool GetSubReg( unsigned Idx,
                  std::string &Name,
                  unsigned &Start,
                  unsigned &End );

  /// Get SubReg name by index
  std::string GetSubRegNameByIndex(unsigned Idx);

  /// Delete SubReg by index
  void DeleteSubRegByIndex(unsigned Idx);

  /// Retrieve the register pseudo name
  std::string GetPseudoName();

  /// Set the register width
  bool SetWidth(int Width);

  /// Set the register attributes
  bool SetAttrs( uint32_t Attr );

  /// Unset the register attributes
  bool UnsetAttrs( uint32_t Attr );

  /// Set the SIMD flag and SIMD lane width
  bool SetSIMD( int width );

  /// Unset the SIMD attributes
  bool UnsetSIMD();

  /// Set the register index
  bool SetIndex( int index );

  /// Set the register sharing flag
  bool SetShared(bool S);

  /// Set the register pseudo name
  bool SetPseudoName( std::string PseudoName );

  /// Insert a new subregister
  bool InsertSubReg( std::string Name, unsigned Start, unsigned End );

  /// Default destructor
  ~CoreGenReg();
};

#endif

// EOF
