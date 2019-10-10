//
// _StatsPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class StatsPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Stats Pass
 *
 */

#ifndef _STATSPASS_H_
#define _STATSPASS_H_

#include <iostream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

class StatsPass : public CoreGenPass{
private:
  unsigned SocNode;         ///< Soc node counter
  unsigned CoreNode;        ///< Core node counter
  unsigned InstFormatNode;  ///< Inst format node counter
  unsigned InstNode;        ///< Inst node counter
  unsigned PseudoInstNode;  ///< Pseudo inst node counter
  unsigned RegClassNode;    ///< Reg class node counter
  unsigned RegNode;         ///< Reg node counter
  unsigned ISANode;         ///< ISA node counter
  unsigned CacheNode;       ///< Cache node counter
  unsigned EncodingNode;    ///< Encoding node counter
  unsigned ExtNode;         ///< Extension node counter
  unsigned CommNode;        ///< Comm node counter
  unsigned SpadNode;        ///< Spad node counter
  unsigned MCtrlNode;       ///< MCtrl node counter
  unsigned VTPNode;         ///< VTP node counter
  unsigned PluginNode;      ///< Plugin node counter

  /// Decode and record the node type
  void RecordNodeType( CoreGenNode *N );

public:
  /// Default constructor
  StatsPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default destructor
  ~StatsPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
