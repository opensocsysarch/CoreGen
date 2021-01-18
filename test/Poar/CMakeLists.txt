# CoreGen test Poar CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_POAR_TESTING)
  set(BUILD_COREGEN_POAR_CLI_TESTING "ON")
  set(BUILD_COREGEN_POAR_TEXT_TESTING "ON")
  set(BUILD_COREGEN_POAR_OVERHEAD_TESTING "ON")
  set(BUILD_COREGEN_POAR_YAML_TESTING "ON")
  set(BUILD_COREGEN_POAR_XML_TESTING "ON")
  set(BUILD_COREGEN_POAR_LATEX_TESTING "ON")
  set(BUILD_COREGEN_POAR_SIGMAP_TESTING "ON")
endif()

add_subdirectory(CLI)
add_subdirectory(Text)
add_subdirectory(Overhead)
add_subdirectory(Yaml)
add_subdirectory(XML)
add_subdirectory(Latex)
add_subdirectory(SigMap)

# EOF
