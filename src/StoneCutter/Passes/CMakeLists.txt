# StoneCutter/Passes CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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
)

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
  core irreader
  mcjit
  ipo
  executionengine
  instrumentation
  transformutils
  vectorize
  scalaropts
  instcombine
  ${llvm_codegens}
  ${llvm_parsers}
)

include_directories(${COREGEN_INCLUDE_PATH})
add_library(PassObjs OBJECT ${PassSrcs})

#-- EOF
