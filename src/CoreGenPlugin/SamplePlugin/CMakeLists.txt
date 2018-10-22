# CoreGen SamplePlugin CMakeLists.txt
#
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(SamplePluginSrcs
SamplePlugin.cpp
SamplePlugin.h
)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

add_library(SamplePlugin SHARED ${SamplePluginSrcs})

target_link_libraries(SamplePlugin CoreGenPluginImpl)

install(TARGETS SamplePlugin DESTINATION plugins/SamplePlugin)
install(DIRECTORY "."
        DESTINATION "plugins/SamplePlugin"
        FILES_MATCHING
        PATTERN "*.h")
