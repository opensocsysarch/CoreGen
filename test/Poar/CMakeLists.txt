# CoreGen test Poar CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_POAR_TESTING)
  set(BUILD_COREGEN_POAR_CLI_TESTING "ON")
endif()

add_subdirectory(CLI)

# EOF
