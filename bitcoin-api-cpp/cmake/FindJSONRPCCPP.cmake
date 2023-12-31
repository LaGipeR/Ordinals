# Finds the JSON-RPC-CPP includes and library
# 
# This module defines
# - JSONCPP_INCLUDE_DIRS, where to find json.h, etc.
# - JSONCPP_LIBRARIES, the libraries needed to use jsoncpp.
# - JSONCPP_FOUND, If false, do not try to use jsoncpp.
# - JSONCPP_LIBRARIES, where to find the jsoncpp library.

FIND_PATH(JSONRPCCPP_INCLUDE_DIRS client.h server.h
        /usr/include
        /usr/local/include
        PATH_SUFFIXES jsonrpccpp
)

FIND_LIBRARY(JSONRPCCPP_LIBRARIES
        NAMES
        jsonrpccpp-common
        jsonrpccpp-client
        jsonrpccpp-server
        PATHS
        /usr/lib
        /usr/local/lib
)

IF (NOT JSONRPCCPP_INCLUDE_DIRS)
    MESSAGE("Could not find include dirs")
ENDIF (NOT JSONRPCCPP_INCLUDE_DIRS)

IF (NOT JSONRPCCPP_LIBRARIES)
    MESSAGE("Could not find libraries")
ENDIF (NOT JSONRPCCPP_LIBRARIES)

IF (JSONRPCCPP_INCLUDE_DIRS AND JSONRPCCPP_LIBRARIES)
    SET(JSONRPCCPP_FOUND TRUE)
ENDIF (JSONRPCCPP_INCLUDE_DIRS AND JSONRPCCPP_LIBRARIES)


IF (JSONRPCCPP_FOUND)
    IF (NOT JSONRPCCPP_FIND_QUIETLY)
        MESSAGE(STATUS "Found JSON-RPC-CPP: ${JSONRPCCPP_LIBRARIES}")
    ENDIF (NOT JSONRPCCPP_FIND_QUIETLY)
ELSE (JSONRPCCPP_FOUND)
    IF (JSONRPCCPP_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find JSON-RPC-CPP library include: ${JSONRPCCPP_INCLUDE_DIRS}, lib: ${JSONRPCCPP_LIBRARIES}")
    ENDIF (JSONRPCCPP_FIND_REQUIRED)
ENDIF (JSONRPCCPP_FOUND)

MARK_AS_ADVANCED(
        JSONRPCCPP_INCLUDE_DIRS
        JSONRPCCPP_LIBRARIES
)

