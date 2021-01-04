# StoneCutter CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(SCCompSrcs
 SCOpts.cpp
 SCExec.cpp
 SCMsg.cpp
 SCLexer.cpp
 SCParser.cpp
 SCLLCodeGen.cpp
 SCChiselCodeGen.cpp
 SCPass.cpp
 SCPipeInfo.cpp
 SCPerf.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

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

message(STATUS "llvm_libs set to ${llvm_libs}")

add_subdirectory(Intrinsics)
add_subdirectory(Passes)

add_library(SCComp ${SCCompSrcs} $<TARGET_OBJECTS:IntrinObjs> $<TARGET_OBJECTS:PassObjs>)

target_link_libraries(SCComp ${llvm_libs} CoreGenSigMap)

install(TARGETS SCComp DESTINATION lib)

#-- EOF
