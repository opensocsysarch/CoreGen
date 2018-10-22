# CoreGen Passes CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_subdirectory(Data)
add_subdirectory(Analysis)

include_directories(${COREGEN_INCLUDE_PATH})

#install(DIRECTORY "."
#        DESTINATION "include/CoreGenBackend/Passes"
#        FILES_MATCHING
#        PATTERN "*.h")
