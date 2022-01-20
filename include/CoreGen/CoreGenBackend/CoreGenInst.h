//
// _CoreGenInst_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenInst
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Instruction Handler
 *
 */

#ifndef _COREGENINST_H_
#define _COREGENINST_H_

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"

class CoreGenInst : public CoreGenNode{
private:
  CoreGenISA *isa;                          ///< CoreGenInst: Instruction Set Bundle
  CoreGenInstFormat *format;                ///< CoreGenInst: Instruction format container
  std::string Impl;                         ///< CoreGenInst: StoneCutter Instruction Implementation
  std::string Syntax;                       ///< CoreGenInst: Instruction printing syntax
  std::vector<CoreGenEncoding *> Encodings; ///< CoreGenInst: Instruction encodings

public:
  /// Default Constructor
  CoreGenInst(CoreGenErrno *E);

  /// Overloaded Constructor
  CoreGenInst( std::string Name, CoreGenISA *ISA, CoreGenInstFormat *Format,
               CoreGenErrno *Errno );

  /// Default Destructor
  ~CoreGenInst();

  /// CoreGenInst: Validates the syntax string
  bool ValidateSyntax(std::string S);

  /// CoreGenInst: Set the instruction format
  bool SetFormat( CoreGenInstFormat *Format );

  /// CoreGenInst: Set a field encoding
  bool SetEncoding( std::string Field, uint64_t value );

  /// CoreGenInst: Set the ISA bundle type
  bool SetISA( CoreGenISA *ISA );

  /// CoreGenInst: Set the instruction printing syntax
  bool SetSyntax( std::string S );

  /// CoreGenInst: Clear all encodings
  void ClearEncodings() { Encodings.clear(); }

  /// CoreGenInst: Retrieve the encoding for the target field
  uint64_t GetEncoding( std::string F, bool *Success );

  /// CoreGenInst: Retrieve the instruction format
  CoreGenInstFormat *GetFormat();

  /// CoreGenInst: Retrieve the number of encodings
  unsigned GetNumEncodings() { return Encodings.size(); }

  /// CoreGenInst: Retrieve the target encoding
  CoreGenEncoding *GetEncoding(unsigned I);

  /// CoreGenInst: Retrieve the target encoding by its field name
  CoreGenEncoding *GetEncoding(std::string S);

  /// CoreGenInst: Retrieve the ISA bundle type
  CoreGenISA *GetISA() { return isa; }

  /// CoreGenInst: Set the StoneCutter implementation logic
  bool SetImpl( std::string I ) { Impl = I; return true; }

  /// CoreGenInst: Get the StoneCutter implementation logic
  std::string GetImpl() { return Impl; }

  /// CoreGenInst: Get the instruction syntax
  std::string GetSyntax() { return Syntax; }

  /// CoreGenInst: Determine if StoneCutter logic is utilized
  bool IsImpl() { if( Impl.length() > 0 ){ return true; }else{ return false;} }

  /// CoreGenInst: Determine if the Syntax has been set
  bool IsSyntax() { if( Syntax.length() > 0 ){ return true; }else{ return false;} }

  /// CoreGenInst: Set a null format for the target instruction
  bool SetNullFormat();

  /// CoreGenInst: Set a null ISA for the target instruction
  bool SetNullISA();
};

#endif

// EOF
