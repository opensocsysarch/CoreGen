# CoreGen Tools Top-Level CMake
#
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#
cmake_minimum_required(VERSION 3.4.3)
project(CoreGen)
set(VERSION "0.2.0")

#------------------------------------------------------------------------
#-- EXTERNAL CMAKE SCRIPTS
#------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake"
  ${CMAKE_MODULE_PATH}
)
include(UseLATEX OPTIONAL)

#------------------------------------------------------------------------
#-- COMPILER MACROS
#------------------------------------------------------------------------
set(COREGEN_COMPILER_MACROS "-DCGIPREFIX=${CMAKE_INSTALL_PREFIX}")

#------------------------------------------------------------------------
#-- COMPILER OPTIONS
#------------------------------------------------------------------------
# Require C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall ${COREGEN_COMPILER_MACROS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall ${COREGEN_COMPILER_MACROS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall ${COREGEN_COMPILER_MACROS}")

# Check for memory sanity checks
if(COREGEN_MEMORY_SANITY)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Memory sanitizier is suitable for use with Clang")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    message(STATUS "Memory sanitizier is suitable for use with AppleClang")
  else()
    message(FATAL_ERROR "Clang is required to build using memory sanitizer.")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

# Set the system type
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOREGEN_PLATFORM=${CMAKE_SYSTEM_NAME}")
message(STATUS "Building for ${CMAKE_SYSTEM_NAME}")

# Set the CoreGen linkage
if( (${CMAKE_SYSTEM_NAME} EQUAL "Darwin") OR (${CMAKE_SYSTEM_NAME} EQUAL "Linux") )
  set(COREGEN_LINK_LIBS "dl")
else()
  set(COREGEN_LINK_LIBS "")
endif()

# Set the clingo build options
set(CLINGO_BUILD_STATIC ON)

#------------------------------------------------------------------------
#-- FIND PACKAGES
#------------------------------------------------------------------------
# Find LLVM packages
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find Doxygen packages
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the doxygen-ized API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

#------------------------------------------------------------------------
#-- COREGEN PATHS
#------------------------------------------------------------------------
# Add package paths
if(NOT COREGEN_INCLUDE_PATH)
  set(COREGEN_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
  message(STATUS "COREGEN_INCLUDE_PATH set to ${COREGEN_INCLUDE_PATH}")
endif()

if(NOT STONECUTTER_PATH)
  set(STONECUTTER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/StoneCutter")
  message(STATUS "STONECUTTER_PATH set to ${STONECUTTER_PATH}")
endif()

if(NOT COREGEN_PATH)
  set(COREGEN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/CoreGenBackend")
  message(STATUS "COREGEN_PATH set to ${COREGEN_PATH}")
endif()

if(NOT COREGEN_PLUGIN_PATH)
  set(COREGEN_PLUGIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/CoreGenPlugin")
  message(STATUS "COREGEN_PLUGIN_PATH set to ${COREGEN_PLUGIN_PATH}")
endif()

if(NOT COREGEN_CODEGEN_PATH)
  set(COREGEN_CODEGEN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/CoreGenCodegen")
  message(STATUS "COREGEN_CODEGEN_PATH set to ${COREGEN_CODEGEN_PATH}")
endif()

if(NOT COREGEN_BUILD_DIR)
  set(COREGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  message(STATUS "COREGEN_BUILD_DIR set to ${COREGEN_BUILD_DIR}")
endif()

if(NOT YAML_PATH)
  set(YAML_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/yaml/yaml-cpp")
  message(STATUS "YAML_PATH set to ${YAML_PATH}")
endif()

if(NOT CLINGO_PATH)
  set(CLINGO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/clingo-5.3.0")
  message(STATUS "CLINGO_PATH set to ${CLINGO_PATH}")
endif()

#------------------------------------------------------------------------
#-- TESTING
#------------------------------------------------------------------------
# Enable testing
enable_testing()

#------------------------------------------------------------------------
#-- DOCUMENTATION
#------------------------------------------------------------------------
# Doxygen config
if(BUILD_DOCUMENTATION)
  if( NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is required to build the documentation.")
  endif()

  set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/CoreGen.conf)

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cleanmanpages.sh ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/man/man3
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/html DESTINATION share/doc)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/man DESTINATION share/man)

endif()

#------------------------------------------------------------------------
#-- PACKAGING
#------------------------------------------------------------------------
#-- builds a standard redhat/centos rpm
if(COREGEN_BUILD_RPM)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "RPM")
  set(CPACK_PACKAGE_NAME "CoreGen")
  set(CPACK_PACKAGE_RELEASE 1)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_RPM_PACKAGE_REQUIRES "llvm-toolset-7 zlib-devel")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

#-- builds a standard debian/ubuntu deb
if(COREGEN_BUILD_DEB)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "DEB")
  set(CPACK_PACKAGE_NAME "CoreGen")
  set(CPACK_PACKAGE_RELEASE 1)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

#-- builds a tgz
if(COREGEN_BUILD_TGZ)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_PACKAGE_NAME "CoreGen")
  set(CPACK_PACKAGE_RELEASE 1)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

#-- builds an osx dmg
if(COREGEN_BUILD_DMG)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "DMG")
  set(CPACK_PACKAGE_NAME "CoreGen")
  set(CPACK_PACKAGE_RELEASE 1)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

#------------------------------------------------------------------------
#-- SUBDIRECTORIES
#------------------------------------------------------------------------
# Add subdirectories
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(test)

# EOF
