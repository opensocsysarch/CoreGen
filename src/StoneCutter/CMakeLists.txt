# StoneCutter CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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
)

include_directories(${COREGEN_INCLUDE_PATH})

set(llvm_codegens
  x86codegen
  AArch64codegen
  ARMcodegen
  Mipscodegen
  PowerPCcodegen
)
set(llvm_parsers
  x86asmparser
  AArch64asmparser
  ARMasmparser
  Mipsasmparser
  PowerPCasmparser
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
  ${llvm_codegens}
  ${llvm_parsers}
)

add_subdirectory(Intrinsics)
add_subdirectory(Passes)

include_directories(${YAML_PATH}/include)

add_library(SCComp ${SCCompSrcs} $<TARGET_OBJECTS:IntrinObjs> $<TARGET_OBJECTS:PassObjs>)

target_link_libraries(SCComp ${llvm_libs} yaml-cpp)

install(TARGETS SCComp DESTINATION lib)

#-- EOF
