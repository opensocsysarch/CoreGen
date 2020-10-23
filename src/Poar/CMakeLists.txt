# src/Poar CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(PoarSrcs
 PoarOpts.cpp
)

# -- TARGETS
add_library(Poar ${PoarSrcs})
add_executable(poar PoarMain.cpp)

# -- HEADERS
include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

# -- LINK LIBS
target_link_libraries(poar Poar CoreGenBackend SCComp)

# -- INSTALL TARGETS
install(TARGETS Poar DESTINATION lib)
install(TARGETS poar DESTINATION bin)

#-- EOF
