# CoreGen test CGCLI CMakeLists.txt
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


file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.sh)

if(BUILD_COREGEN_CGCLI_TESTING)
set(BUILD_COREGEN_CGCLI_SPECDOC_TESTING "ON")
foreach(testSrc ${TEST_SRCS})
  # Extract the file name
  get_filename_component(testName ${testSrc} NAME_WE)

  # Add the tests for execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${BASH_EXECUTABLE} ${testSrc} ${COREGEN_BUILD_DIR}/src/CGCLI )
  set_tests_properties( ${testName} PROPERTIES PASS_REGULAR_EXPRESSION "")
endforeach(testSrc)
endif()

add_subdirectory(SpecDocs)
