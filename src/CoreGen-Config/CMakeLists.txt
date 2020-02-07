# CoreGen-Config CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(CoreGenConfig
CoreGen-Config.cpp
)

add_executable(coregen-config ${CoreGenConfig})
include_directories("${PROJECT_BINARY_DIR}")

install(TARGETS coregen-config DESTINATION bin)
