# StoneCutter/Intrinsics CMakeLists.txt
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(IntrinSrcs
SCMax.cpp
SCMin.cpp
SCLoad.cpp
SCStore.cpp
SCLoadElem.cpp
SCStoreElem.cpp
SCClz.cpp
SCCtz.cpp
SCNot.cpp
SCNeg.cpp
SCPopcount.cpp
SCReverse.cpp
SCSext.cpp
SCZext.cpp
SCRotateL.cpp
SCRotateR.cpp
SCMaj.cpp
SCDoz.cpp
SCCompress.cpp
SCCompressM.cpp
SCInsertS.cpp
SCInsertZ.cpp
SCExtractS.cpp
SCExtractZ.cpp
SCMerge.cpp
SCConcat.cpp
SCLss.cpp
SCFence.cpp
SCBsel.cpp
SCNop.cpp
)

llvm_map_components_to_libnames(llvm_libs all)

include_directories(${COREGEN_INCLUDE_PATH})
add_library(IntrinObjs OBJECT ${IntrinSrcs})

#-- EOF
