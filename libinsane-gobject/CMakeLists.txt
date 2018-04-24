project(Libinsane-gobject)

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

find_package(PkgConfig)

include(GNUInstallDirs)
include(FindGLib)
include(FindGObjectIntrospection)
include(GObjectIntrospectionMacros)

include_directories(
  SYSTEM
  ${GLIB2_INCLUDE_DIRS}
)

add_definitions(${GLIB2_CFLAGS_OTHER})

set(LIBINSANE_GOBJECT_SRCS
    src/libinsane-api.c
)
set(LIBINSANE_GOBJECT_HEADERS
    include/libinsane-gobject/libinsane-api.h
)

add_library(libinsane_gobject ${LIBINSANE_GOBJECT_SRCS})
target_link_libraries(libinsane_gobject ${GLIB2_LIBRARIES})
target_include_directories(libinsane_gobject PUBLIC include)

install(TARGETS libinsane_gobject
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

install(FILES
    ${LIBINSANE_GOBJECT_HEADERS}
    DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/libinsane-gobject)

macro(_list_prefix _outvar _listvar _prefix)
  set(${_outvar})
  foreach(_item IN LISTS ${_listvar})
    list(APPEND ${_outvar} ${_prefix}${_item})
  endforeach()
endmacro(_list_prefix)

set(INTROSPECTION_SCANNER_ARGS "--add-include-path=${CMAKE_CURRENT_SOURCE_DIR} --warn-all")
set(INTROSPECTION_COMPILER_ARGS "--includedir=${CMAKE_CURRENT_SOURCE_DIR}")

set(introspection_files ${LIBINSANE_GOBJECT_SRCS} ${LIBINSANE_GOBJECT_HEADERS})
set(Libinsane_0_1_gir "libinsane-gobject")
set(Libinsane_0_1_gir_INCLUDES GObject-2.0 Gio-2.0 cairo-1.0)
get_directory_property(_tmp_includes INCLUDE_DIRECTORIES)
_list_prefix(_includes _tmp_includes "-I")
set(Libinsane_0_1_gir_CFLAGS ${_includes} -L${CMAKE_BINARY_DIR})
set(Libinsane_0_1_gir_LIBS libinsane_gobject)
_list_prefix(_abs_introspection_files introspection_files "${CMAKE_CURRENT_SOURCE_DIR}/")
set(Libinsane_0_1_gir_FILES ${_abs_introspection_files})
set(Libinsane_0_1_gir_SCANNERFLAGS --c-include poppler.h)
set(Libinsane_0_1_gir_EXPORT_PACKAGES poppler-glib)

gir_add_introspections(Libinsane-0.1.gir)
