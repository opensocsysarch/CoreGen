# include/CoreGen/CoreGenCodegen CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_subdirectory(NodeCodegen)
add_subdirectory(LLVMCodegen)

install(DIRECTORY "."
        DESTINATION "include/CoreGen/CoreGenCodegen"
        FILES_MATCHING
        PATTERN "*.h")
