# StoneCutter CMakeLists.txt
#
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
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
# SCAST.cpp
 SCLLCodeGen.cpp
)

include_directories(${COREGEN_INCLUDE_PATH})

llvm_map_components_to_libnames(llvm_libs support core irreader)

add_library(SCComp ${SCCompSrcs})

target_link_libraries(SCComp ${llvm_libs})

install(TARGETS SCComp DESTINATION lib)

#-- EOF
