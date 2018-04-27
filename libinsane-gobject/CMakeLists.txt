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

set(INSANE_GOBJECT_SRCS
    src/item.c
    src/libinsane-api.c
    src/scan_parameters.c
)
set(INSANE_GOBJECT_HEADERS
    include/libinsane-gobject/item.h
    include/libinsane-gobject/libinsane-api.h
    include/libinsane-gobject/scan_parameters.h
)

add_library(insane_gobject SHARED ${INSANE_GOBJECT_SRCS})
target_link_libraries(insane_gobject ${GLIB2_LIBRARIES})
target_include_directories(insane_gobject PUBLIC include)

install(TARGETS insane_gobject
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

macro(_list_prefix _outvar _listvar _prefix)
  set(${_outvar})
  foreach(_item IN LISTS ${_listvar})
    list(APPEND ${_outvar} ${_prefix}${_item})
  endforeach()
endmacro(_list_prefix)

set(INTROSPECTION_SCANNER_ARGS "--add-include-path=${CMAKE_CURRENT_SOURCE_DIR} --warn-all --warn-error")
set(INTROSPECTION_COMPILER_ARGS "--includedir=${CMAKE_CURRENT_SOURCE_DIR}")

set(introspection_files ${INSANE_GOBJECT_SRCS} ${INSANE_GOBJECT_HEADERS})
set(Libinsane_0_1_gir "libinsane-gobject")
set(Libinsane_0_1_gir_INCLUDES GObject-2.0)
get_directory_property(_tmp_includes INCLUDE_DIRECTORIES)
_list_prefix(_includes _tmp_includes "-I")
set(Libinsane_0_1_gir_CFLAGS ${_includes} -L${CMAKE_BINARY_DIR})
set(Libinsane_0_1_gir_LIBS insane_gobject)
_list_prefix(_abs_introspection_files introspection_files "${CMAKE_CURRENT_SOURCE_DIR}/")
set(Libinsane_0_1_gir_FILES ${_abs_introspection_files})
set(Libinsane_0_1_gir_SCANNERFLAGS)
set(Libinsane_0_1_gir_EXPORT_PACKAGES libinsane-gobject)

set(INTROSPECTION_GIRS)
list(APPEND INTROSPECTION_GIRS Libinsane-0.1.gir)
gir_add_introspections(INTROSPECTION_GIRS)
