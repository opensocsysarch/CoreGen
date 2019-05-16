# CoreGen SigMap CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(SigMapSrcs
CoreGenSigMap.cpp
SCSig.cpp
SCTmp.cpp
)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

add_library(CoreGenSigMap ${SigMapSrcs})
target_link_libraries(CoreGenSigMap yaml-cpp)

install(TARGETS CoreGenSigMap DESTINATION lib)
# EOF
