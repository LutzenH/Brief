cmake_minimum_required(VERSION 3.20)
project(Brief C)

set(CMAKE_C_STANDARD 99)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DBRIEF_DEBUG)
else ()
    add_definitions(-DBRIEF_RELEASE)
endif ()

IF (WIN32)
    add_definitions(-DBRIEF_WINDOWS)
ELSEIF (APPLE)
    add_definitions(-DBRIEF_MACOS)
ELSEIF (UNIX)
    add_definitions(-DBRIEF_LINUX)
ENDIF ()

set(BRIEF_FILES
        src/brief_types.h
        src/brief_platform.c
        src/brief_platform.h
        src/brief_kdtree.c
        src/brief_kdtree.h
        src/brief_visualization.c
        src/brief_visualization.h)

add_executable(Brief src/brief.c ${BRIEF_FILES})
