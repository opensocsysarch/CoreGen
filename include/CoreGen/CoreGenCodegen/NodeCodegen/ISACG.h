//
// _ISACG_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

class ISACG : public CoreGenNodeCodegen {
private:
  /// Writes the ISA and its instructions out to instructions.chisel
  bool WriteISA(std::ofstream &O,
                std::vector<CoreGenInst *> Insts,
                std::vector<CoreGenInstFormat *> InstFormats);

  /// Returns a Chisel tabular bit pattern for the target instruction
  std::string GetInstBitPat(CoreGenInstFormat *Format, CoreGenInst *Inst);

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
