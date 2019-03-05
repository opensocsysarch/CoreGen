# CGCLI CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

#-- set the version numbers
set( CGCLI_VERSION_MAJOR 1 )
set( CGCLI_VERSION_MINOR 0 )
set( CGCLI_VERSION_PATCH 0 )

#-- configure the target header file
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/CGCLIConfig.h.in"
  "${PROJECT_BINARY_DIR}/CGCLIConfig.h"
)

set(CGCLISrcs
CGCLIOpts.cpp
CGCLIOpts.h
CGCLIMain.cpp
)

add_executable(cgcli ${CGCLISrcs})
include_directories(${COREGEN_INCLUDE_PATH})
include_directories("${PROJECT_BINARY_DIR}")
#include_directories(${COREGEN_PATH})
#include_directories(${COREGEN_PATH}/Passes)
#include_directories(${COREGEN_PLUGIN_PATH})
#include_directories(${STONECUTTER_PATH})
include_directories(${YAML_PATH}/include)
target_link_libraries(cgcli SCComp CoreGenBackend yaml-cpp
                      ${COREGEN_LINK_LIBS})
install(TARGETS cgcli DESTINATION bin)
