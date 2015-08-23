cmake_minimum_required(VERSION 3.0)

PROJECT("muhkuh_gui")


# Enable tests.
ENABLE_TESTING()


# Select the LUA version.
INCLUDE(cmake/muhkuh_lua.cmake)

# Generate a version number from a file.
INCLUDE(cmake/muhkuh_version.cmake)

# Set some installation folders.
INCLUDE(cmake/muhkuh_installation_folders.cmake)


#----------------------------------------------------------------------------
#
# Set custom definitions for several compiler.
#

# Microsoft: Do not show msvs security warnings about unsafe functions.
IF(${CMAKE_BUILD_TOOL} MATCHES "(msdev|VCExpress|devenv|nmake)")
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF(${CMAKE_BUILD_TOOL} MATCHES "(msdev|VCExpress|devenv|nmake)")


# Microsoft: Set warning level to 2.
IF(${CMAKE_BUILD_TOOL} MATCHES "(msdev|VCExpress|devenv)")
	ADD_DEFINITIONS(/W2)
ENDIF(${CMAKE_BUILD_TOOL} MATCHES "(msdev|VCExpress|devenv)")


#----------------------------------------------------------------------------
#
# Include the projects in the subdirectories.
#

ADD_SUBDIRECTORY(application)


#----------------------------------------------------------------------------

