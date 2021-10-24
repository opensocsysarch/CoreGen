# src/Poar CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(PoarSrcs
 PoarOpts.cpp
 PoarConfig.cpp
 PoarData.cpp
 PoarIO.cpp
)

# -- TARGETS
add_library(PoarBackend ${PoarSrcs} $<TARGET_OBJECTS:AccumObjs>)
add_executable(poar PoarMain.cpp)

# -- HEADERS
include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

# -- LINK LIBS
target_link_libraries(PoarBackend CoreGenBackend CoreGenSigMap yaml-cpp ${COREGEN_LINK_LIBS})
target_link_libraries(poar PoarBackend ${COREGEN_LINK_LIBS})

# -- INSTALL TARGETS
install(TARGETS PoarBackend DESTINATION lib)
install(TARGETS poar DESTINATION bin)

add_subdirectory(Accumulators)

#-- EOF
