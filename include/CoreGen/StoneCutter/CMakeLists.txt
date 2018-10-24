# include/CoreGen/StoneCutter CMakeLists.txt
#
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

install(DIRECTORY "."
        DESTINATION "include/StoneCutter"
        FILES_MATCHING
        PATTERN "*.h")
