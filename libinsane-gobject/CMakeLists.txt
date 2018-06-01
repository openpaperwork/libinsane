project(Libinsane-gobject)

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -Wunused -g -ggdb -O2")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -Wunused -g -ggdb -O2")

find_package(PkgConfig)
find_program(GLIB2_MKENUMS glib-mkenums)

include(GNUInstallDirs)
include(FindGLib)
include(FindGObjectIntrospection)
include(GObjectIntrospectionMacros)

message(STATUS "INTROSPECTION_FOUND: ${INTROSPECTION_FOUND}")
message(STATUS "INTROSPECTION_COMPILER: ${INTROSPECTION_COMPILER}")
message(STATUS "INTROSPECTION_SCANNER: ${INTROSPECTION_SCANNER}")

include_directories(
  SYSTEM
  ${GLIB2_INCLUDE_DIRS}
  ../libinsane/include
)
link_directories(../libinsane/build build)

add_definitions(${GLIB2_CFLAGS_OTHER})

set(INSANE_GOBJECT_SRCS
    generated/enums.c
    src/device_descriptor.c
    src/error.c
    src/item.c
    src/libinsane-api.c
    src/log.c
    src/option_descriptor.c
    src/scan_parameters.c
    src/scan_session.c
)
set(INSANE_GOBJECT_HEADERS
    include/libinsane-gobject/capability.h
    include/libinsane-gobject/constraint.h
    include/libinsane-gobject/device_descriptor.h
    include/libinsane-gobject/error.h
    include/libinsane-gobject/img_format.h
    include/libinsane-gobject/item.h
    include/libinsane-gobject/item_type.h
    include/libinsane-gobject/libinsane-api.h
    include/libinsane-gobject/log.h
    include/libinsane-gobject/option_descriptor.h
    include/libinsane-gobject/scan_parameters.h
    include/libinsane-gobject/scan_session.h
    include/libinsane-gobject/unit.h
)

## Generate enums.h / enums.c

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated/enums.h
  COMMAND ${GLIB2_MKENUMS}
  ARGS
    --template src/enums.h.template
    ${INSANE_GOBJECT_HEADERS} > ${CMAKE_CURRENT_SOURCE_DIR}/generated/enums.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${INSANE_GOBJECT_HEADERS}
          ${CMAKE_CURRENT_SOURCE_DIR}/src/enums.h.template
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated/enums.c
  COMMAND ${GLIB2_MKENUMS}
  ARGS
    --template src/enums.c.template
    ${INSANE_GOBJECT_HEADERS} > ${CMAKE_CURRENT_SOURCE_DIR}/generated/enums.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${INSANE_GOBJECT_HEADERS}
          ${CMAKE_CURRENT_SOURCE_DIR}/generated/enums.h
          ${CMAKE_CURRENT_SOURCE_DIR}/src/enums.c.template
)

## Build libinsane_gobject itself

add_library(insane_gobject SHARED ${INSANE_GOBJECT_SRCS})
target_link_libraries(insane_gobject insane ${GLIB2_LIBRARIES})
target_include_directories(insane_gobject PUBLIC include generated)

install(TARGETS insane_gobject
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES
  ${INSANE_GOBJECT_HEADERS}
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/enums.h
  DESTINATION include/libinsane-gobject)

## Introspection scanner --> .gir / .typelib

macro(_list_prefix _outvar _listvar _prefix)
  set(${_outvar})
  foreach(_item IN LISTS ${_listvar})
    list(APPEND ${_outvar} ${_prefix}${_item})
  endforeach()
endmacro(_list_prefix)

set(INTROSPECTION_SCANNER_ARGS "--add-include-path=${CMAKE_CURRENT_SOURCE_DIR}")
set(INTROSPECTION_COMPILER_ARGS "--includedir=${CMAKE_CURRENT_SOURCE_DIR}")

set(introspection_files ${INSANE_GOBJECT_SRCS} ${INSANE_GOBJECT_HEADERS})
set(Libinsane_0_1_gir "libinsane-gobject")
set(Libinsane_0_1_gir_INCLUDES GObject-2.0)
get_directory_property(_tmp_includes INCLUDE_DIRECTORIES)
_list_prefix(_includes _tmp_includes "-I")
set(Libinsane_0_1_gir_CFLAGS
  ${_includes}
  -L${CMAKE_BINARY_DIR}
  -L${CMAKE_CURRENT_SOURCE_DIR}/../libinsane/build
  -linsane
)
set(Libinsane_0_1_gir_LIBS insane_gobject)
_list_prefix(_abs_introspection_files introspection_files "${CMAKE_CURRENT_SOURCE_DIR}/")
set(Libinsane_0_1_gir_FILES ${_abs_introspection_files})
set(Libinsane_0_1_gir_SCANNERFLAGS "--warn-all" "--warn-error")
set(Libinsane_0_1_gir_EXPORT_PACKAGES libinsane-gobject)

set(INTROSPECTION_GIRS)
list(APPEND INTROSPECTION_GIRS Libinsane-0.1.gir)
gir_add_introspections(INTROSPECTION_GIRS)

## Doc

# WORKAROUND(JFlesch):
# https://bugzilla.gnome.org/show_bug.cgi?id=792148
set(exec_prefix /usr)

find_package(GtkDoc 1.25 REQUIRED)

file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/doc/gtkdocentities.ent
  DESTINATION ${CMAKE_BINARY_DIR}/libinsane_gobject/xml
)
# WORKAROUND(JFlesch):
# gtk-doc seems to accept only one LDPATH ?
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/../libinsane/build/libinsane.so
  DESTINATION ${CMAKE_BINARY_DIR}
)

gtk_doc_add_module(
  libinsane_gobject
  XML ${CMAKE_SOURCE_DIR}/doc/libinsane_gobject-gtkdoc.xml
  SOURCE ${CMAKE_SOURCE_DIR}
  SUFFIXES "h" "c"
  LIBRARIES insane_gobject
#  LDPATH ${CMAKE_CURRENT_SOURCE_DIR}/../libinsane/build ${CMAKE_CURRENT_SOURCE_DIR}/build
)

add_custom_target(doc DEPENDS doc-libinsane_gobject)
