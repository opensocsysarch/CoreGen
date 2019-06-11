# Install script for directory: /home/fconlon/Documents/CoreGen/test/DAG/IndividualPasses/KnownPass

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/StatsPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/MultSoCPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/DanglingRegionPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/DanglingNodePass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/EncodingCollisionPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/RegIdxPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/RegSafetyPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/CacheLevelPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/ICacheCheckerPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/L1SharedPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/RegFieldPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/CoreSafetyPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/CommSafetyPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/MandatoryFieldPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/EncodingGapPass/cmake_install.cmake")
  include("/home/fconlon/Documents/CoreGen/build/test/DAG/IndividualPasses/KnownPass/PInstSafetyPass/cmake_install.cmake")

endif()

