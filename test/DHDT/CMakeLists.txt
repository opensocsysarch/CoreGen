# CoreGen test DHDT CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_DHDT_TESTING)
  set(BUILD_COREGEN_DHDT_CLI_TESTING "ON")
  set(BUILD_COREGEN_DHDT_POWER_TESTING "ON")
endif()

add_subdirectory(CLI)
add_subdirectory(Power)

# EOF
