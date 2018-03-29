cmake_minimum_required(VERSION 3.7)

project(Libinsane)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

aux_source_directory(src/libinsane LIBINSANE_SRCS)
add_library(libinsane ${LIBINSANE_SRCS})

target_include_directories(libinsane PUBLIC include)

if (WIN32)
    MESSAGE(STATUS "Operating system: Microsoft Windows")
endif (WIN32)

if (UNIX)
    MESSAGE(STATUS "Operating system: Unix-like")
    include(GtkDoc)
    include(Sane)
endif(UNIX)
