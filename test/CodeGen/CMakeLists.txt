# CoreGen test/CodeGen CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

if(BUILD_COREGEN_CODEGEN_TESTING)
  set(BUILD_COREGEN_CODEGEN_STONECUTTERIMPL_TESTING "ON")
endif()

add_subdirectory(StoneCutterImpl)

#-- EOF
