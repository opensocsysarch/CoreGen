# CoreGen test/StoneCutter CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_STONECUTTER_TESTING)
  set(BUILD_COREGEN_STONECUTTER_CLI_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_PARSER_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_OPTPARSER_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_OPTPARSER_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_CODEGEN_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_SIGMAP_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_SCSIGV_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_PIPELINER_TESTING "ON")
endif()

add_subdirectory(CLI)
add_subdirectory(OptParser)
add_subdirectory(Parser)
add_subdirectory(CodeGen)
add_subdirectory(SigMap)
add_subdirectory(SCSigV)
add_subdirectory(Pipeliner)

#-- EOF
