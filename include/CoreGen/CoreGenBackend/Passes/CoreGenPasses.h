//
// _CoreGenPasses_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenPasses
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Pass Headers
 *
 */

#ifndef _COREGENPASSES_H_
#define _COREGENPASSES_H_

/// CoreGen Data Passes
#include "CoreGen/CoreGenBackend/Passes/Data/StatsPass.h"
#include "CoreGen/CoreGenBackend/Passes/Data/SpecDoc.h"

/// CoreGen Analysis Passes
#include "CoreGen/CoreGenBackend/Passes/Analysis/ICacheCheckerPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/CacheLevelPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/DanglingNodePass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/DanglingRegionPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/EncodingCollisionPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/RegIdxPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/RegFieldPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/RegSafetyPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/CoreSafetyPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/RegClassSafetyPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/SpadSafetyPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/L1SharedPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/MultSoCPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/MandatoryFieldPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/EncodingGapPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/SafeDeletePass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/CommSafetyPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/PInstSafetyPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/CommSafetyPass.h"
#include "CoreGen/CoreGenBackend/Passes/Analysis/ASPSolverPass.h"

// CoreGen Optimization Passes
#include "CoreGen/CoreGenBackend/Passes/Opt/RegClassOpt.h"
#include "CoreGen/CoreGenBackend/Passes/Opt/CacheLevelOpt.h"

#endif

// EOF
