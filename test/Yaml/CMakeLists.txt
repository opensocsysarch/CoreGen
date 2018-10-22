# CoreGen test Yaml CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# CoreGen can not be copied and/or distributed without the
# express permission of Tactical Computing Laboratories, LLC
#

if(BUILD_COREGEN_YAML_TESTING)
  set(BUILD_COREGEN_YAML_READER_TESTING "ON")
  set(BUILD_COREGEN_YAML_WRITER_TESTING "ON")
endif()

add_subdirectory(Writer)
add_subdirectory(Reader)

# EOF
