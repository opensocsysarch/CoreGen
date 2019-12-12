# CoreGen Tools Top-Level CMake
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
#-- DERIVE THE RELEASE CODENAME
#------------------------------------------------------------------------
execute_process(COMMAND lsb_release -cs
    OUTPUT_VARIABLE COREGEN_RELEASE_CODENAME
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

#------------------------------------------------------------------------
#-- CLINGO OPTIONS
#------------------------------------------------------------------------
set(CLINGO_BUILD_STATIC ON CACHE BOOL "Forcing Clingo to build static libs")
set(CLINGO_MANAGE_RPATH OFF CACHE BOOL "Disable Clingo management of RPATH")
set(CLINGO_BUILD_APPS OFF CACHE BOOL "Disabling Clingo Apps")
set(LIB_POTASSCO_INSTALL_LIB ON CACHE BOOL "Forcing Potassco install")
set(CLASP_INSTALL_LIB ON CACHE BOOL "Forcing Clasp install")

#------------------------------------------------------------------------
#-- COMPILER MACROS
#------------------------------------------------------------------------
set(CLINGO_COMPILER_MACROS "-DCLINGO_WITH_LUA") 
set(COREGEN_COMPILER_MACROS "-DCGIPREFIX=${CMAKE_INSTALL_PREFIX} ${CLINGO_COMPILER_MACROS}")

#------------------------------------------------------------------------
#-- COMPILER OPTIONS
#------------------------------------------------------------------------
# Require C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14 -Wall ${COREGEN_COMPILER_MACROS}")
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
  unset(CMAKE_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "-O1 -g -std=c++14 -Wall -shared-libstdc++ ${COREGEN_COMPILER_MACROS} -fsanitize=address -fno-omit-frame-pointer")
  message(STATUS "CMAKE_CXX_FLAGS set to ${CMAKE_CXX_FLAGS}")
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
  set(YAML_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/yaml/yaml-cpp-0.6.2")
  message(STATUS "YAML_PATH set to ${YAML_PATH}")
endif()

if(NOT CLINGO_PATH)
  set(CLINGO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/clingo/clingo-5.3.0")
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
  set(CPACK_RPM_PACKAGE_REQUIRES "llvm-toolset-7 zlib-devel re2c lua lua-devel devtoolset-7-gcc-c++")
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

  #-- setup package deps
  if( "${COREGEN_RELEASE_CODENAME}" STREQUAL "bionic" )
    message(STATUS "Setting package dependencies for Ubuntu 18.04 (bionic)")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "clang-6.0 lldb-6.0 lld-6.0 libllvm6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-doc llvm-6.0-examples llvm-6.0-runtime re2c lua5.3 liblua5.3-dev")
  elseif( "${COREGEN_RELEASE_CODENAME}" STREQUAL "xenial" )
    message(STATUS "Setting package dependencies for Ubuntu 16.04 (xenial)")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libllvm-6.0-ocaml-dev libllvm6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-doc llvm-6.0-examples llvm-6.0-runtime clang-6.0 clang-tools-6.0 clang-6.0-doc libclang-common-6.0-dev libclang-6.0-dev libclang1-6.0 clang-format-6.0 python-clang-6.0 re2c lua5.3 liblua5.3-dev")
  else()
    message(STATUS "Unknown debian version: ${COREGEN_RELEASE_CODENAME}")
    message(STATUS "No debian package dependencies are set!!!")
  endif()

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
add_subdirectory(archive)
add_subdirectory(test)

# EOF
