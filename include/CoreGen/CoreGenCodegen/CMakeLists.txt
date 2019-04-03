# include/CoreGen/CoreGenCodegen CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_subdirectory(NodeCodegen)

install(DIRECTORY "."
        DESTINATION "include/CoreGenCodegen"
        FILES_MATCHING
        PATTERN "*.h")
