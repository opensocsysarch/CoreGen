# CoreGen test/StoneCutter/CodeGen CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_STONECUTTER_CODEGEN_TESTING)
  set(BUILD_COREGEN_STONECUTTER_CODEGEN_KNOWNFAIL_TESTING "ON")
endif()

add_subdirectory(KnownFail)

#-- EOF
