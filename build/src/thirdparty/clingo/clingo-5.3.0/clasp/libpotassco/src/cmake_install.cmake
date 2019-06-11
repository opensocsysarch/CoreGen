# Install script for directory: /home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/potassco-1.0/potassco" TYPE FILE FILES
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/application.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/aspif.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/aspif_text.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/basic_types.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/clingo.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/convert.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/match_basic_types.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/platform.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/rule_utils.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/smodels.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/string_convert.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/theory_data.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/potassco-1.0/potassco/program_opts" TYPE FILE FILES
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/errors.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/mapped_value.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/program_options.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/typed_value.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/value.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/value_store.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/potassco-1.0/potassco/program_opts/detail" TYPE FILE FILES
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/detail/notifier.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/detail/refcountable.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/libpotassco/potassco/program_opts/detail/value_store.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/potassco-1.0" TYPE STATIC_LIBRARY FILES "/home/fconlon/Documents/CoreGen/build/lib/libpotassco.a")
endif()

