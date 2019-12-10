# include/CoreGen/StoneCutter CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_subdirectory(Intrinsics)
add_subdirectory(Passes)

install(DIRECTORY "."
        DESTINATION "include/CoreGen/StoneCutter"
        FILES_MATCHING
        PATTERN "*.h")
