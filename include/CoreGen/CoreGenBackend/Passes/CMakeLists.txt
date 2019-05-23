# include/CoreGen/CoreGenBackend/Passes CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_subdirectory(Analysis)
add_subdirectory(Data)
add_subdirectory(Opt)

install(DIRECTORY "."
        DESTINATION "CoreGen/include/CoreGenBackend/Passes"
        FILES_MATCHING
        PATTERN "*.h")
