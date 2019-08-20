//
// _CoreGenInst_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

  bool ValidateSyntax(std::string S);       ///< CoreGenInst: Validates the syntax string

public:
  /// Default Constructor
  CoreGenInst(CoreGenErrno *E);

  /// Overloaded Constructor
  CoreGenInst( std::string Name, CoreGenISA *ISA, CoreGenInstFormat *Format,
               CoreGenErrno *Errno );

  /// Default Destructor
  ~CoreGenInst();

  /// Set the instruction format
  bool SetFormat( CoreGenInstFormat *Format );

  /// Set a field encoding
  bool SetEncoding( std::string Field, uint64_t value );

  /// Set the ISA bundle type
  bool SetISA( CoreGenISA *ISA );

  /// Set the instruction printing syntax
  bool SetSyntax( std::string S );

  /// Retrieve the encoding for the target field
  uint64_t GetEncoding( std::string F, bool *Success );

  /// Retrieve the instruction format
  CoreGenInstFormat *GetFormat();

  /// Retrieve the number of encodings
  unsigned GetNumEncodings() { return Encodings.size(); }

  /// Retrieve the target encoding
  CoreGenEncoding *GetEncoding(unsigned I);

  /// Retrieve the target encoding by its field name
  CoreGenEncoding *GetEncoding(std::string S);

  /// Retrieve the ISA bundle type
  CoreGenISA *GetISA() { return isa; }

  /// Set the StoneCutter implementation logic
  bool SetImpl( std::string I ) { Impl = I; return true; }

  /// Get the StoneCutter implementation logic
  std::string GetImpl() { return Impl; }

  /// Get the instruction syntax
  std::string GetSyntax() { return Syntax; }

  /// Determine if StoneCutter logic is utilized
  bool IsImpl() { if( Impl.length() > 0 ){ return true; }else{ return false;} }

  /// Determine if the Syntax has been set
  bool IsSyntax() { if( Syntax.length() > 0 ){ return true; }else{ return false;} }

  /// Set a null format for the target instruction
  bool SetNullFormat();

  /// Set a null ISA for the target instruction
  bool SetNullISA();
};

#endif

// EOF
