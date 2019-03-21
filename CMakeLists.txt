
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

# See http://git.libssh.org/projects/libssh.git/tree/ for example of a good project configuration.
# Additional CMake modules, specific for this project
SET(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/CMake/Modules
)

# Load those modules
INCLUDE(Define_CMakeDefaults)
INCLUDE(Define_PlatformDefaults)
INCLUDE(Define_CompilerFlags)

INCLUDE(Macro_EnsureOutOfSourceBuild)

# Nifty marco, which forbids in source build. (source: http://git.libssh.org/projects/libssh.git)
MACRO_ENSURE_OUT_OF_SOURCE_BUILD("${PROJECT_NAME}  requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${CMAKE_PROJECT_NAME} [options]' there.")

# Project command is here, to set some values correctly.
SET(CMAKE_PROJECT_NAME "rdg")
PROJECT(${CMAKE_PROJECT_NAME})

# Version information
SET(EXECUTABLE_VERSION_MAJOR "0")
SET(EXECUTABLE_VERSION_MINOR "1")
SET(EXECUTABLE_VERSION_PATCH "0")

SET(EXECUTABLE_VERSION "${EXECUTABLE_VERSION_MAJOR}.${EXECUTABLE_VERSION_MINOR}.${EXECUTABLE_VERSION_PATCH}")

# Global vars
SET(RDG_LIBRARY_NAME "rdg")

MESSAGE(STATUS "Current built type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Current build type flags: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")
MESSAGE(STATUS "Configuring application ${APPLICATION_NAME}")

# MACRO_PREPARE_SOURCES(PROJECT_SOURCES)
SET(RDG_SRC

	include/rdg/dungeontypes.h
	src/dungeontypes.cpp
	include/rdg/dungeongenerator.h
	src/dungeongenerator.cpp
	include/rdg/dungeontools.h
	src/dungeontools.cpp
)

#-----------------------------------------------------------------------------
ADD_LIBRARY(
    ${RDG_LIBRARY_NAME}
    ${RDG_SRC}
)

TARGET_INCLUDE_DIRECTORIES(
    ${RDG_LIBRARY_NAME}

    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include	
)

#-----------------------------------------------------------------------------
ADD_EXECUTABLE(
    rdg_sample
	
    main.cpp
)

TARGET_LINK_LIBRARIES(
	rdg_sample
	
	${RDG_LIBRARY_NAME}
)
