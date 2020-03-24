# CoreGenCodegen/NodeCodegen CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(NodeCodegenSrcs
RegClassCG.cpp
SpadCG.cpp
SocCG.cpp
ISACG.cpp
PluginCG.cpp
DPathCG.cpp
CacheCG.cpp
)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

add_library(NodeCodegenObjs OBJECT ${NodeCodegenSrcs})

# EOF
