# CoreGen test DAG CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_DAG_INDIVIDUALPASS_TESTING)
  set(BUILD_COREGEN_DAG_INDIVIDUALPASS_KNOWNPASS_TESTING "ON")
  set(BUILD_COREGEN_DAG_INDIVIDUALPASS_KNOWNFAIL_TESTING "ON")
endif()

add_subdirectory(KnownPass)
add_subdirectory(KnownFail)

# EOF
