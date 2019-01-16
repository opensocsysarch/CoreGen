# CoreGen Plugin CMakeLists.txt
#
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(PluginSrcs
CoreGenPluginImpl.cpp
)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

include_directories(${CLINGO_PATH}/libclingo)
include_directories(${CLINGO_PATH}/libgringo)
include_directories(${CLINGO_PATH}/libluaclingo)

add_library(CoreGenPluginImpl ${PluginSrcs})

target_link_libraries(CoreGenPluginImpl CoreGenBackend CoreGenCodegen yaml-cpp)
target_compile_options(CoreGenPluginImpl PRIVATE -fPIC)
add_subdirectory(SamplePlugin)

install(TARGETS CoreGenPluginImpl DESTINATION lib)
install(DIRECTORY "."
        DESTINATION "include"
        FILES_MATCHING
        PATTERN "*.h")
