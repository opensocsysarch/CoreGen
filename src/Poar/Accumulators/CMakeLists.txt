# src/Poar/Accumulators CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(AccumSrcs
PoarRegAccum.cpp
)


# -- HEADERS
include_directories(${COREGEN_INCLUDE_PATH})

# -- TARGETS
add_library(AccumObjs OBJECT ${AccumSrcs})

#-- EOF
