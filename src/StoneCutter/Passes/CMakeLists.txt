# StoneCutter/Passes CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(PassSrcs
SCInstArg.cpp
SCInstFormat.cpp
SCIOWarn.cpp
SCFieldIO.cpp
SCPipeBuilder.cpp
SCSigMap.cpp
SCMetadataAdjust.cpp
SCVLIW.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

add_library(PassObjs OBJECT ${PassSrcs})

#-- EOF
