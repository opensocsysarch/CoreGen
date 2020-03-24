# CoreGen Codegen CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(CodegenSrcs
CoreGenCodegen.cpp
CoreGenChiselCodegen.cpp
CoreGenLLVMCodegen.cpp
CoreGenNodeCodegen.cpp
CoreGenChiselParms.cpp
CoreGenCompCodegen.cpp
)

add_subdirectory(NodeCodegen)
add_subdirectory(LLVMCodegen)

include_directories(${YAML_PATH}/include)
include_directories(${COREGEN_INCLUDE_PATH})

add_library(CoreGenCodegen ${CodegenSrcs}
            $<TARGET_OBJECTS:NodeCodegenObjs>
            $<TARGET_OBJECTS:LLVMCodegenObjs> )

target_link_libraries(CoreGenCodegen CoreGenBackend CoreGenPluginImpl CoreGenSigMap CoreGenArchive)

install(TARGETS CoreGenCodegen DESTINATION lib)
# EOF
