# include/CoreGen/CoreGenBackend CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_subdirectory(Passes)

install(DIRECTORY "."
        DESTINATION "include/CoreGen/CoreGenBackend"
        FILES_MATCHING
        PATTERN "*.h")
