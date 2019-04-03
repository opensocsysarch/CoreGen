# CoreGen test/DAG/IndividualPass/KnownPass/RegFieldPass CMakeLists.txt
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

find_program (BASH_EXECUTABLE bash)
if (BASH_EXECUTABLE)
  message(STATUS "FOUND BASH AT ${BASH_EXECUTABLE}")
else ()
  message(FATAL_ERROR "COULD NOT FIND BASH SHELL")
endif ()


file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.yaml)

if(BUILD_COREGEN_DAG_INDIVIDUALPASS_KNOWNPASS_REGFIELDPASS_TESTING)
foreach(testSrc ${TEST_SRCS})
  # Extract the file name
  get_filename_component(testName ${testSrc} NAME_WE)

  # Add the tests for execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/sc_RegFieldPass_test.sh ${COREGEN_BUILD_DIR}/src/CGCLI ${testSrc})
  set_tests_properties( ${testName} PROPERTIES PASS_REGULAR_EXPRESSION "")
endforeach(testSrc)
endif()

