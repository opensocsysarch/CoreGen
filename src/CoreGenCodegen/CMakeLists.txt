# CoreGen Codegen CMakeLists.txt
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(CodegenSrcs
CoreGenCodegen.cpp
CoreGenChiselCodegen.cpp
CoreGenLLVMCodegen.cpp
)

add_subdirectory(NodeCodegen)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

add_library(CoreGenCodegen ${CodegenSrcs} $<TARGET_OBJECTS:NodeCodegenObjs>)

target_link_libraries(CoreGenCodegen CoreGenBackend CoreGenPluginImpl)

install(TARGETS CoreGenCodegen DESTINATION lib)
# EOF
