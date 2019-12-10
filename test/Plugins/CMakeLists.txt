# CoreGen test Plugins CMakeLists.txt
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

include_directories(${COREGEN_INCLUDE_PATH})
include_directories(${YAML_PATH}/include)

include_directories(${CLINGO_PATH}/libclingo)
include_directories(${CLINGO_PATH}/libgringo)
include_directories(${CLINGO_PATH}/libluaclingo)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(COREGEN_TEST_PLUGIN "libSamplePlugin.dylib")
else()
  set(COREGEN_TEST_PLUGIN "libSamplePlugin.so")
endif()

message(STATUS "COREGEN_TEST_PLUGIN set to ${COREGEN_TEST_PLUGIN}")

if(BUILD_COREGEN_PLUGIN_TESTING)
foreach(testSrc ${TEST_SRCS})
  # Extract the file name
  get_filename_component(testName ${testSrc} NAME_WE)

  # Add compile target
  add_executable(${testName} ${testSrc})

  # Add linker deps
  target_link_libraries(${testName} CoreGenBackend dl)

  # Drop the exe's in a separate directory
  set_target_properties(${testName} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)

  # Add the tests for execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName} ${COREGEN_BUILD_DIR}/src/CoreGenPlugin/SamplePlugin/${COREGEN_TEST_PLUGIN})
  set_tests_properties( ${testName} PROPERTIES PASS_REGULAR_EXPRESSION "")
endforeach(testSrc)
endif()
