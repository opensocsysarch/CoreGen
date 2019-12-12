# SCSigV CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_executable(scsigv SCSigV.cpp)

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

target_link_libraries(scsigv CoreGenSigMap yaml-cpp)

install(TARGETS scsigv DESTINATION bin)

#-- EOF
