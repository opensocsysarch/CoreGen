# CoreGen Analysis Passes CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(AnalysisPassSrcs
ICacheCheckerPass.cpp
DanglingRegionPass.cpp
DanglingNodePass.cpp
EncodingCollisionPass.cpp
CacheLevelPass.cpp
RegIdxPass.cpp
RegFieldPass.cpp
RegSafetyPass.cpp
CoreSafetyPass.cpp
L1SharedPass.cpp
MultSoCPass.cpp
MandatoryFieldPass.cpp
EncodingGapPass.cpp
RegClassSafetyPass.cpp
SafeDeletePass.cpp
CommSafetyPass.cpp
PInstSafetyPass.cpp
CommSafetyPass.cpp
ASPSolverPass.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})
add_library(AnalysisPassObjs OBJECT ${AnalysisPassSrcs})

#-- EOF
