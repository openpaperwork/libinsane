cmake_minimum_required(VERSION 3.5)

set(HAS_TOPLEVEL_CMAKELISTS TRUE)

add_subdirectory(libinsane)

include("${CMAKE_SOURCE_DIR}/libinsane-gobject/cmake/FindGLib.cmake")

if(GLIB2_FOUND)
add_subdirectory(libinsane-gobject)
endif(GLIB2_FOUND)
