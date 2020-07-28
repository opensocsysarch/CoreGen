//
// _CoreGenInstFormat_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenInstFormat
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Instruction Format Hanlder
 *
 */

#ifndef _COREGENINSTFORMAT_H_
#define _COREGENINSTFORMAT_H_

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <map>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"

class CoreGenInstFormat : public CoreGenNode{
public:
  /// CoreGenInstFormat: Instruction field types
  typedef enum{
    CGInstReg       = 0,  ///< CGInstField: Register field
    CGInstCode      = 1,  ///< CGInstField: Instruction code field (opcode, etc)
    CGInstImm       = 2,  ///< CGInstField: Immediate field
    CGInstUnk       = 64  ///< CGInstField: Unknown field type
  }CGInstField;       ///< CoreGenInstFormat: Instruction field types

  /// Default constructor
  CoreGenInstFormat(std::string Name, CoreGenISA *ISA, CoreGenErrno *E);

  /// Default Destructor
  ~CoreGenInstFormat() {}

  /// Insert a filed in the instruction format
  bool InsertField( std::string Name, unsigned StartBit,
                    unsigned EndBit, CGInstField Type, bool Mand );

  /// Insert a field mapping to a register class
  bool InsertRegFieldMap( std::string Name, CoreGenRegClass *RClass );

  /// Set the start bit for the target field
  bool SetStartBit( std::string Name, unsigned Start );

  /// Set the end bit for the target field
  bool SetEndBit( std::string Name, unsigned End );

  /// Set the field type for the target field
  bool SetFieldType( std::string Name, CGInstField Type );

  /// Set the mandatory flag for the target field
  bool SetMandatoryFlag( std::string Name, bool Flag );

  /// Set Register Field as a destination type
  bool SetRegFieldIsDestFlag( std::string Name, bool Flag);

  /// Determine whether the target field is valid
  bool IsValidField( std::string Field );

  /// Get the size of the instruction format (in bits)
  unsigned GetFormatWidth();

  /// Get the target field width (in bits)
  unsigned GetFieldWidth( std::string Field );

  /// Retrieve the field start bit
  unsigned GetStartBit(std::string Name);

  /// Retrieve the mandatory flag
  bool GetMandatoryFlag(std::string Name);

  /// Retrieve the field end bit
  unsigned GetEndBit(std::string Name);

  /// Retrieve the number of mandatory fields
  unsigned GetNumMandatory();

  /// Retrieve the field name
  std::string GetFieldName( unsigned N );

  /// Retrieve the field type
  CoreGenInstFormat::CGInstField GetFieldType(std::string Name);

  /// Converts CGInstField to std::string
  std::string CGInstFieldToStr(CGInstField F);

  /// Converts std::string to CGInstFiled
  CoreGenInstFormat::CGInstField StrToCGInstField(std::string Field);

  /// Retrieve the register class (IFF Type=CGInstReg)
  CoreGenRegClass *GetFieldRegClass(std::string Name);

  /// Sets the target field in the instruction format to null
  bool SetNullField(std::string Name);

  /// Retrieve the number of instruction format fields
  unsigned GetNumFields() { return Format.size(); }

  /// Retrieve the instruction set type
  CoreGenISA *GetISA() { return ISA; }

  /// Set the instruction set type
  bool SetISA( CoreGenISA *ISA );

  /// Determine if the register field is a destination type
  bool GetRegFieldIsDest( std::string Name);

private:
  std::vector<std::tuple< std::string,
                          unsigned,
                          unsigned,
                          CoreGenInstFormat::CGInstField,
                          bool,
                          bool >> Format;  ///< Format vector of name:start_bit:end:bit:type:mandatory:reg_is_dest
  std::map<std::string,CoreGenRegClass *> RegMap;   ///< Register mapping
  CoreGenISA *ISA;                                  ///< Instructon Set Bundle

  typedef enum {
    CGFormatName      = 0,
    CGFormatStartBit  = 1,
    CGFormatEndBit    = 2,
    CGFormatType      = 3,
    CGFormatMandatory = 4,
    CGFormatRegIsDest = 5
  } CGFormatVectorInd;
};

#endif

// EOF
