# CoreGen Passes CMakeLists.txt
# Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

add_subdirectory(Data)
add_subdirectory(Analysis)
add_subdirectory(Opt)

include_directories(${COREGEN_INCLUDE_PATH})

# EOF
