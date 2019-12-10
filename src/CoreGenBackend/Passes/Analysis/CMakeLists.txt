# CoreGen Analysis Passes CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
SpadSafetyPass.cpp
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
AST.cpp
ConstrParser.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${CLINGO_PATH}/libclingo)
include_directories(${CLINGO_PATH}/libgringo)
include_directories(${CLINGO_PATH}/libluaclingo)
add_library(AnalysisPassObjs OBJECT ${AnalysisPassSrcs})

#-- EOF
