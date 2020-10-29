# CoreGen test CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

find_program (BASH_EXECUTABLE bash)
if (BASH_EXECUTABLE)
  message(STATUS "FOUND BASH AT ${BASH_EXECUTABLE}")
else ()
  message(FATAL_ERROR "COULD NOT FIND BASH SHELL")
endif ()

if(BUILD_ALL_TESTING)
  set(BUILD_COREGEN_YAML_TESTING "ON")
  set(BUILD_COREGEN_DAG_TESTING "ON")
  set(BUILD_COREGEN_CGCLI_TESTING "ON")
  set(BUILD_COREGEN_PLUGIN_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_TESTING "ON")
  set(BUILD_COREGEN_CODEGEN_TESTING "ON")
  set(BUILD_COREGEN_ARCHIVE_TESTING "ON")
  set(BUILD_COREGEN_CONFIG_TESTING "ON")
  set(BUILD_COREGEN_POAR_TESTING "ON")
endif()

add_subdirectory(Yaml)
add_subdirectory(DAG)
add_subdirectory(CGCLI)
add_subdirectory(Plugins)
add_subdirectory(StoneCutter)
add_subdirectory(CodeGen)
add_subdirectory(Archive)
add_subdirectory(Config)
add_subdirectory(Poar)

#-- EOF
