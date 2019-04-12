# CoreGen test/StoneCutter/Parser/KnownPass CMakeLists.txt
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.sc)

if(BUILD_COREGEN_STONECUTTER_PARSER_KNOWNPASS_TESTING)
set(BUILD_COREGEN_STONECUTTER_PARSER_KNOWNPASS_MULTIFILE_TESTING "ON")
foreach(testSrc ${TEST_SRCS})
  # Extract the file name
  get_filename_component(testName ${testSrc} NAME_WE)

  # Add the tests for execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/sc_parser_test.sh ${COREGEN_BUILD_DIR}/src/SCComp ${testSrc})
  set_tests_properties( ${testName} PROPERTIES PASS_REGULAR_EXPRESSION "")
endforeach(testSrc)
endif()

add_subdirectory(MultiFile)

