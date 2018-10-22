# CoreGen test CMakeLists.txt
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# CoreGen can not be copied and/or distributed without the
# express permission of Tactical Computing Laboratories, LLC
#

if(BUILD_ALL_TESTING)
  set(BUILD_COREGEN_YAML_TESTING "ON")
  set(BUILD_COREGEN_DAG_TESTING "ON")
  set(BUILD_COREGEN_CGCLI_TESTING "ON")
  set(BUILD_COREGEN_PLUGIN_TESTING "ON")
endif()

add_subdirectory(Yaml)
add_subdirectory(DAG)
add_subdirectory(CGCLI)
add_subdirectory(Plugins)
