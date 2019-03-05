# CoreGen Data Passes CMakeLists.txt
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(DataPassSrcs
StatsPass.cpp
InstTable.cpp
SpecDoc.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})
add_library(DataPassObjs OBJECT ${DataPassSrcs})

#install(DIRECTORY "."
#        DESTINATION "include/Passes/Data"
#        FILES_MATCHING
#        PATTERN "*.h")
