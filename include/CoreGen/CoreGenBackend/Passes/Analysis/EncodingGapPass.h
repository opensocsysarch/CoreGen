//
// _EncodingGapPass_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class EncodingGapPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Encoding Gap Checker Pass
 *
 */

#ifndef _ENCODINGGAPPASS_H_
#define _ENCODINGGAPPASS_H_

#include <iostream>
#include <string>
#include <vector>

#include "CoreGen/CoreGenBackend/CoreGenPass.h"
#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenInstFormat.h"

class EncodingGapPass : public CoreGenPass{
private:
  /// Examines the encoding gaps
  bool ExamineGap( CoreGenDAG *D, CoreGenInstFormat *IF );

  /// Examines the specific undefined fields
  void ExamineFieldGap( unsigned Width,
                        std::vector<unsigned> StartBits,
                        std::vector<unsigned> EndBits );
public:
  /// Default Constructor
  EncodingGapPass(std::ostream *O, CoreGenDAG *D, CoreGenErrno *E);

  /// Default Destructor
  ~EncodingGapPass();

  /// Execute the pass
  virtual bool Execute() override;
};

#endif

// EOF
