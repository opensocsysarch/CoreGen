# CoreGen Optimization Passes CMakeLists.txt
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(OptPassSrcs
RegClassOpt.cpp
CacheLevelOpt.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})
add_library(OptPassObjs OBJECT ${OptPassSrcs})

#-- EOF
