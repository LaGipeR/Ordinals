# -*- cmake -*-
# - Find JSONCpp
# Find the JSONCpp includes and library
# This module defines
#  JSONCPP_INCLUDE_DIRS, where to find json.h, etc.
#  JSONCPP_LIBRARIES, the libraries needed to use jsoncpp.
#  JSONCPP_FOUND, If false, do not try to use jsoncpp.
#  also defined, but not for general use are
#  JSONCPP_LIBRARIES, where to find the jsoncpp library.

FIND_PATH(JSONCPP_INCLUDE_DIRS json.h
        /usr/include
        /usr/local/include
        ${CMAKE_SOURCE_DIR}/win32-deps/include
        PATH_SUFFIXES jsoncpp/json jsoncpp
)


FIND_LIBRARY(JSONCPP_LIBRARIES NAMES jsoncpp HINTS /usr/lib /usr/local/lib ${CMAKE_SOURCE_DIR}/win32-deps/lib)

IF (JSONCPP_LIBRARIES AND JSONCPP_INCLUDE_DIRS)
    SET(JSONCPP_LIBRARIES ${JSONCPP_LIBRARIES})
    SET(JSONCPP_FOUND "YES")
ELSE (JSONCPP_LIBRARIES AND JSONCPP_INCLUDE_DIRS)
    SET(JSONCPP_FOUND "NO")
ENDIF (JSONCPP_LIBRARIES AND JSONCPP_INCLUDE_DIRS)


IF (JSONCPP_FOUND)
    IF (NOT JSONCPP_FIND_QUIETLY)
        MESSAGE(STATUS "Found JSONCpp: ${JSONCPP_LIBRARIES}")
    ENDIF (NOT JSONCPP_FIND_QUIETLY)
ELSE (JSONCPP_FOUND)
    IF (JSONCPP_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find JSONCPP library include: ${JSONCPP_INCLUDE_DIRS}, lib: ${JSONCPP_LIBRARIES}")
    ENDIF (JSONCPP_FIND_REQUIRED)
ENDIF (JSONCPP_FOUND)

# Deprecated declarations.
SET(NATIVE_JSONCPP_INCLUDE_PATH ${JSONCPP_INCLUDE_DIRS})
GET_FILENAME_COMPONENT(NATIVE_JSONCPP_LIB_PATH ${JSONCPP_LIBRARIES} PATH)

MARK_AS_ADVANCED(
        JSONCPP_LIBRARIES
        JSONCPP_INCLUDE_DIRS
)

