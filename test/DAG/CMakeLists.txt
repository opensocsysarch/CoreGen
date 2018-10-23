# CoreGen test DAG CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_DAG_TESTING)
  set(BUILD_COREGEN_DAG_DOT_TESTING "ON")
  set(BUILD_COREGEN_DAG_FULLPASS_TESTING "ON")
  set(BUILD_COREGEN_DAG_INDIVIDUALPASS_TESTING "ON")
endif()

add_subdirectory(DOT)
add_subdirectory(FullPasses)
add_subdirectory(IndividualPasses)

# EOF
