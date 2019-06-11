# Note: Adapted from Jonathan Müller's blog post on supporting CMake install:
# https://foonathan.github.io/blog/2016/03/03/cmake-install.html
#
# checks version: major must match, minor must be less than or equal

set(PACKAGE_VERSION 3.3.4)

if("${PACKAGE_FIND_VERSION_MAJOR}" EQUAL "3")
    if ("${PACKAGE_FIND_VERSION_MINOR}" EQUAL "3")
        set(PACKAGE_VERSION_EXACT TRUE)
    elseif(NOT ("3" EQUAL 0)
           AND "${PACKAGE_FIND_VERSION_MINOR}" LESS "3")
        set(PACKAGE_VERSION_COMPATIBLE TRUE)
    else()
        set(PACKAGE_VERSION_UNSUITABLE TRUE)
    endif()
else()
    set(PACKAGE_VERSION_UNSUITABLE TRUE)
endif()
