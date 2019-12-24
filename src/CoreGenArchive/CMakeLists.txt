# CoreGen Archive CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(ArchiveSrcs
CoreGenArchive.cpp
)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

add_library(CoreGenArchive ${ArchiveSrcs})
#target_link_libraries(CoreGenArchive yaml-cpp curl git2)
target_link_libraries(CoreGenArchive yaml-cpp curl)

install(TARGETS CoreGenArchive DESTINATION lib)
# EOF
