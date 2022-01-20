# SCComp CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_executable(sccomp SCComp.cpp)

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

target_link_libraries(sccomp SCComp)

install(TARGETS sccomp DESTINATION bin)

#-- EOF
