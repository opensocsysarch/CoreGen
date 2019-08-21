//
// _CoreGenPseudoInst_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenPseudoInst
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Psuedo Instruction Handler
 *
 */

#ifndef _COREGENPSEUDOINST_H_
#define _COREGENPSEUDOINST_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"

class CoreGenPseudoInst : public CoreGenNode{
private:
  std::string Syntax;                       ///< CoreGenPseudoInst: Instruction printing syntax
  std::vector<CoreGenEncoding *> Encodings; ///< CoreGenPseudoInst: Encoding value vector
  CoreGenInst *Inst;                        ///< CoreGenPseudoInst: Mapping instruction definition
  CoreGenISA *ISA;                          ///< CoreGenPseudoInst: Instruction set bundle

  bool ValidateSyntax(std::string S);        ///< CoreGenPseudoInt: Validates the syntax string

public:

  /// Default Constructor
  CoreGenPseudoInst(std::string InstName,
                    CoreGenInst *BaseInst,
                    CoreGenErrno *E);

  /// Default Destructor
  ~CoreGenPseudoInst();

  /// Set the field mapping for the pseudo inst
  bool SetEncoding( std::string Field, uint64_t Value );

  /// Get the encoding for the target field
  uint64_t GetEncoding( std::string Field );

  /// Retrieve the number of encodings
  unsigned GetNumEncodings() { return Encodings.size(); }

  /// Retrieve the target encoding
  CoreGenEncoding *GetEncoding( unsigned I );

  /// Set the target instruction
  bool SetTargetInst( CoreGenInst *Inst );

  /// Get the target instruction set bundle
  CoreGenISA *GetISA() { return ISA; }

  /// Sets the ISA to null
  bool SetNullISA() { ISA = nullptr; return true; }

  /// Get the target instruction mapping
  CoreGenInst *GetInst() { return Inst; }

  /// Sets the instruction to null
  bool SetNullInst();

  /// Sets the instruction syntax
  bool SetSyntax(std::string S);

  /// Retrieves the instruction syntax
  std::string GetSyntax() { return Syntax; }

  /// Determine if the Syntax has been set
  bool IsSyntax() { if( Syntax.length() > 0 ){ return true; }else{ return false;} }
};

#endif

// EOF
