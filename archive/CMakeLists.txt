# archive CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

install(DIRECTORY "."
        DESTINATION "archive"
        FILES_MATCHING
        PATTERN "*.yaml")
