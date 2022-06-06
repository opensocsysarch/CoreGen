# src/DHDT CMakeLists.txt
#
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(DHDTSrcs
  DHDTOpts.cpp
  DHDTGraph.cpp
)

llvm_map_components_to_libnames(llvm_libs
  support
  core
  irreader
  mcjit
  ipo
  executionengine
  instrumentation
  transformutils
  vectorize
  scalaropts
  instcombine
  bitreader
  bitwriter
  native)

# -- TARGETS
add_library(DHDTBackend ${DHDTSrcs})
add_executable(dhdt DHDTMain.cpp)

# -- HEADERS
include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

# -- LINK LIBS
target_link_libraries(dhdt DHDTBackend CoreGenBackend SCComp CoreGenSigMap
                      ${llvm_libs} ${COREGEN_LINK_LIBS})

# -- INSTALL TARGETS
install(TARGETS DHDTBackend DESTINATION lib)
install(TARGETS dhdt DESTINATION bin)

#-- EOF
