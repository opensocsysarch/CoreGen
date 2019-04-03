# CoreGenBackend CMakeLists.txt
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(CoreGenSrcs
CoreGenBackend.cpp
CoreGenEncoding.cpp
CoreGenPseudoInst.cpp
CoreGenISA.cpp
CoreGenCache.cpp
CoreGenCore.cpp
CoreGenSpad.cpp
CoreGenMCtrl.cpp
CoreGenDAG.cpp
CoreGenErrno.cpp
CoreGenExt.cpp
CoreGenInst.cpp
CoreGenInstFormat.cpp
CoreGenNode.cpp
CoreGenReg.cpp
CoreGenRegClass.cpp
CoreGenSoC.cpp
CoreGenComm.cpp
CoreGenVTP.cpp
CoreGenProj.cpp
CoreGenYaml.cpp
CoreGenPluginMgr.cpp
CoreGenPlugin.cpp
CoreGenPassMgr.cpp
CoreGenEnv.cpp
)

add_subdirectory(Passes)

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

include_directories(${CLINGO_PATH}/libclingo)
include_directories(${CLINGO_PATH}/libgringo)
include_directories(${CLINGO_PATH}/libluaclingo)

link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
add_library(CoreGenBackend ${CoreGenSrcs} $<TARGET_OBJECTS:DataPassObjs>
                                          $<TARGET_OBJECTS:AnalysisPassObjs>
                                          $<TARGET_OBJECTS:OptPassObjs>)
target_link_libraries(CoreGenBackend CoreGenPluginImpl CoreGenCodegen yaml-cpp libclingo libpyclingo libluaclingo ${COREGEN_LINK_LIBS})
target_compile_options(CoreGenBackend PRIVATE -fPIC)

install(TARGETS CoreGenBackend DESTINATION lib)

#-- EOF
