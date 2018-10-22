//
// _SpecDoc_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SpecDoc
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen SpecDoc Pass
 *
 */

#ifndef _SPECDOC_H_
#define _SPECDOC_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"
#include "CoreGen/CoreGenBackend/CoreGenInst.h"
#include "CoreGen/CoreGenBackend/CoreGenPseudoInst.h"
#include "CoreGen/CoreGenBackend/CoreGenRegClass.h"
#include "CoreGen/CoreGenBackend/CoreGenReg.h"
#include "CoreGen/CoreGenBackend/CoreGenISA.h"
#include "CoreGen/CoreGenBackend/CoreGenEncoding.h"

class SpecDoc : public CoreGenPass{
private:
  /// Write out the spec document makefile
  bool WriteMakefile();

  /// Write out the body of the spec document
  bool WriteTexfile(CoreGenDAG *D);

  /// Write the overview tex output
  bool WriteOverviewTex( CoreGenDAG *D, std::ofstream &O);

  /// Write the register class tex output
  bool WriteRegisterClassTex( CoreGenDAG *D, std::ofstream &O );

  /// Write the instruction format tex output
  bool WriteInstFormatTex( CoreGenDAG *D, std::ofstream &O );

  /// Write the instruction tex output
  bool WriteInstTex( CoreGenDAG *D, std::ofstream &O );

  /// Write the pseudo instruction tex output
  bool WritePseudoInstTex( CoreGenDAG *D, std::ofstream &O );

  /// Write the instruction table tex
  bool WriteInstTableTex( CoreGenDAG *D, std::ofstream &O );

  /// Generate the LaTeX table headers for inst formats
  std::string GenerateInstFormatHeader(CoreGenInstFormat *IF);

public:
  /// Default constructor
  SpecDoc(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~SpecDoc();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
