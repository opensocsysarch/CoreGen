# CoreGen test/StoneCutter/CLI CMakeLists.txt
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.sh)

if(BUILD_COREGEN_STONECUTTER_CLI_TESTING)
foreach(testSrc ${TEST_SRCS})
  # Extract the file name
  get_filename_component(testName ${testSrc} NAME_WE)

  # Add the tests for execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${BASH_EXECUTABLE} ${testSrc} ${COREGEN_BUILD_DIR}/src/SCComp)
  set_tests_properties( ${testName} PROPERTIES PASS_REGULAR_EXPRESSION "")
endforeach(testSrc)
endif()

