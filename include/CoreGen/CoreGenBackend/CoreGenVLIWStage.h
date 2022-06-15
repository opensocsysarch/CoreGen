//
// _CoreGenVLIWStage_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenVLIWStage
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen VLIW Stage Node Handler
 *
 */

#ifndef _COREGENVLIWSTAGE_H_
#define _COREGENVLIWSTAGE_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"

class CoreGenVLIWStage : public CoreGenNode{
private:
  unsigned Id;                            ///< VLIW Stage Id (monotonically increasing)
  CoreGenInstFormat *Format;              ///< VLIW Stage instruction format
  std::string Impl;                       ///< VLIW Stage implementation
  std::vector<std::string> Inputs;        ///< VLIW Stage inputs
  std::vector<std::string> Outputs;       ///< VLIW Stage outputs

  /// Parses the VLIW Stage name and derives the stage ID
  bool ParseVLIWStage(std::string Name);

public:
  /// Default constructor
  CoreGenVLIWStage(std::string Name, CoreGenInstFormat *Format, CoreGenErrno *E);

  /// Default destructor
  ~CoreGenVLIWStage();

  /// Retrieve the instruction format
  CoreGenInstFormat *GetFormat() { return Format; }

  /// Retrieve the stage Id (monotonically increasing integer)
  unsigned GetID() { return Id; }

  /// Retrieve the number of inputs
  unsigned GetNumInputs() { return Inputs.size(); }

  /// Retrieve the number of outputs
  unsigned GetNumOutputs() { return Outputs.size(); }

  /// Retrieve the vector of inputs
  std::vector<std::string> GetInputs() { return Inputs; }

  /// Retrieve the vector of outputs
  std::vector<std::string> GetOutputs() { return Outputs; }

  /// Set the instruction format
  bool SetFormat(CoreGenInstFormat *F);

  /// Set the Id
  bool SetID(unsigned I);

  /// Insert an input
  bool InsertInput(std::string I);

  /// Insert an output
  bool InsertOutput(std::string O);

  /// Delete an input by its name
  bool DeleteInputByName(std::string N);

  /// Delete an output by its name
  bool DeleteOutputByName(std::string O);

  /// Set the instruction format to null
  bool SetNullFormat();

  /// Set the StoneCutter implementation logic
  bool SetImpl( std::string I ) { Impl = I; return true; }

  /// Retrieve the StoneCutter implementation logic
  std::string GetImpl() { return Impl; }

  /// Determine whether there is implementation logic present
  bool IsImpl() { if( Impl.length() > 0 ){ return true; } return false; }
};

#endif

// EOF
