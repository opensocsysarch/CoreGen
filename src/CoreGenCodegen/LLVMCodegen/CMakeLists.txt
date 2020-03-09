# CoreGenCodegen/LLVMCodegen CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(LLVMCodegenSrcs
  LLVM801CG.cpp
)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

add_library(LLVMCodegenObjs OBJECT ${LLVMCodegenSrcs})

# EOF
