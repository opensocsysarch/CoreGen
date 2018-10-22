//
// _CoreGenEncoding_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenEncoding
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Instruction Encoding
 *
 */

#ifndef _COREGENENCODING_H_
#define _COREGENENCODING_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"

class CoreGenEncoding : public CoreGenNode{
private:
  uint64_t Encoding;    ///< Encoded Value
  unsigned Len;         ///< Length of Encoding in Bits
  std::string Field;    ///< Field Name

public:
  /// Default Constructor
  CoreGenEncoding( CoreGenErrno *E);

  /// Overloaded Constructor
  CoreGenEncoding( uint64_t Value, unsigned Len,
                   std::string Field, std::string Inst, CoreGenErrno *E );

  /// Default Destructor
  ~CoreGenEncoding();

  /// Set the encoding
  bool SetEncoding( uint64_t Value, unsigned Len,
                    std::string Field, std::string Inst );

  /// Get the encoding field name
  std::string GetField() { return Field; }

  /// Get the zero-extended encoding value
  uint64_t GetEncoding() { return Encoding; }

  /// Get the field length (in bits)
  unsigned GetLength() { return Len; }
};

#endif

// EOF
