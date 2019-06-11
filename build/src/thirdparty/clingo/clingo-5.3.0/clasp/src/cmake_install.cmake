# Install script for directory: /home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/src

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/clasp-3.3.4" TYPE STATIC_LIBRARY FILES "/home/fconlon/Documents/CoreGen/build/lib/libclasp.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/clasp-3.3.4/clasp" TYPE FILE FILES
    "/home/fconlon/Documents/CoreGen/build/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/config.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/heuristics.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/statistics.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/claspfwd.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/logic_program.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/pod_vector.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/weight_constraint.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/asp_preprocessor.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/enumerator.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/clingo.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/lookahead.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/shared_context.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/solver_strategies.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/solve_algorithms.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/literal.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/satelite.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/program_builder.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/dependency_graph.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/clause.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/cb_enumerator.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/minimize_constraint.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/unfounded_check.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/solver_types.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/solver.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/parser.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/clasp_facade.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/model_enumerators.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/logic_program_types.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/constraint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/clasp-3.3.4/clasp/util" TYPE FILE FILES
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/misc_types.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/multi_queue.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/left_right_sequence.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/pod_vector.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/hash.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/indexed_priority_queue.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/type_manip.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/util/timer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/clasp-3.3.4/clasp/cli" TYPE FILE FILES
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/cli/clasp_cli_configs.inl"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/cli/clasp_cli_options.inl"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/cli/clasp_output.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/cli/clasp_options.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/clasp-3.3.4/clasp/mt" TYPE FILE FILES
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/mt/thread.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/mt/mutex.h"
    "/home/fconlon/Documents/CoreGen/src/thirdparty/clingo/clingo-5.3.0/clasp/clasp/mt/parallel_solve.h"
    )
endif()

