# SCComp CMakeLists.txt
#
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_executable(sccomp SCComp.cpp)

#include_directories(../StoneCutter)

include_directories(${COREGEN_INCLUDE_PATH})

target_link_libraries(sccomp SCComp)

#-- EOF
