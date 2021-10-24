# CoreGenSigMap/Passes CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(SMPassSrcs
SMLegalizeSigMap.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})

add_library(SMPassObjs OBJECT ${SMPassSrcs})

#-- EOF
