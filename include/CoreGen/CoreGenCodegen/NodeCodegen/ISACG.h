//
// _ISACG_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class ISACG
 *
 * \ingroup CoreGenCodegen
 *
 * \brief CoreGenNodeCodegen derived class for instruction sets
 *
 */

#ifndef _ISACG_H_
#define _ISACG_H_

#include <bitset>
#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

class ISACG : public CoreGenNodeCodegen {
private:
  /// Writes the ISA and its instructions out to instructions.chisel
  bool WriteISA(std::ofstream &O,
                std::vector<CoreGenInst *> Insts,
                std::vector<CoreGenInstFormat *> InstFormats);

  /// Returns a Chisel tabular bit pattern for the target instruction
  std::string GetInstBitPat(CoreGenInstFormat *Format,
                            CoreGenInst *Inst,
                            bool *Success);

  /// Converts an unsigned 64bit integer to a string of binary digits of length 'len'
  std::string U8ToBinary(uint64_t n, size_t len);

public:
  /// Default constructor
  ISACG(CoreGenNode *N,
         CoreGenProj *P,
         std::string Package,
         std::string Path,
         bool Common,
         CoreGenErrno *E );

  /// Default destructor
  ~ISACG();

  /// Execute the codegen
  virtual bool Execute() { return false; }

  /// Execute the codegen with the target instructions and instruction formats
  bool Execute( std::vector<CoreGenInst *> Insts,
                std::vector<CoreGenInstFormat *> InstFormats );
};

#endif

// EOF
