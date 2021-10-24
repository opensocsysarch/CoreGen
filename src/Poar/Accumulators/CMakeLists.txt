# src/Poar/Accumulators CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(AccumSrcs
PoarRegAccum.cpp
PoarSpadAccum.cpp
PoarCacheAccum.cpp
PoarALURegAccum.cpp
PoarALUAccum.cpp
PoarALUCPathAccum.cpp
PoarALUDPathAccum.cpp
PoarROMAccum.cpp
PoarDPathAccum.cpp
PoarCPathAccum.cpp
)


# -- HEADERS
include_directories(${COREGEN_INCLUDE_PATH})

# -- TARGETS
add_library(AccumObjs OBJECT ${AccumSrcs})

#-- EOF
