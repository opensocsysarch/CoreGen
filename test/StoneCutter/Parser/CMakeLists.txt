# CoreGen test/StoneCutter/Parser CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_STONECUTTER_PARSER_TESTING)
  set(BUILD_COREGEN_STONECUTTER_PARSER_KNOWNPASS_TESTING "ON")
  set(BUILD_COREGEN_STONECUTTER_PARSER_KNOWNFAIL_TESTING "ON")
endif()

add_subdirectory(KnownPass)
add_subdirectory(KnownFail)

#-- EOF
