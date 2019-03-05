# CoreGen test/StoneCutter/OptParser CMakeLists.txt
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_STONECUTTER_OPTPARSER_TESTING)
  set(BUILD_COREGEN_STONECUTTER_OPTPARSER_KNOWNPASS_TESTING "ON")
endif()

add_subdirectory(KnownPass)

#-- EOF
