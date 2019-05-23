# include/CoreGen/CoreGenCodegen/NodeCodegen CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#


install(DIRECTORY "."
        DESTINATION "CoreGen/include/CoreGenCodegen/NodeCodegen"
        FILES_MATCHING
        PATTERN "*.h")
