cmake_minimum_required(VERSION 3.5)

project(Libinsane)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(LIBINSANE_SRCS
    src/bases/dumb.c
    src/log.c
    src/normalizers/all_opts_on_all_sources.c
    src/normalizers/end_of_feed.c
    src/normalizers/flatbed_and_feeder_behavior.c
    src/normalizers/min_one_source.c
    src/normalizers/raw.c
    src/normalizers/raw24.c
    src/normalizers/resolution_constraint_format.c
    src/normalizers/scan_area_opts.c
    src/normalizers/source_names.c
    src/normalizers/source_nodes.c
    src/normalizers/source_types.c
    src/normalizers/sources_order.c
    src/normalizers/strip_non_scanners.c
    src/safebet.c
    src/safebet.c
    src/str2impls.c
    src/workarounds/clean_dev_model_char.c
    src/workarounds/clean_dev_model_from_manufacturer.c
    src/workarounds/dedicated_thread.c
    src/workarounds/no_read_on_inactive_opt.c
    src/workarounds/no_write_on_readonly_opt.c
    src/workarounds/opt_doc_source.c
    src/workarounds/opt_mode.c
    src/workarounds/opt_scan_resolution.c
    src/workarounds/opts_page_size.c
    src/workarounds/strip_translations.c
)
set(LIBINSANE_HEADERS
    include/capi.h
    include/dumb.h
    include/error.h
    include/log.h
    include/multiplexer.h
    include/normalizers.h
    include/safebet.h
    include/str2impls.h
    include/util.h
    include/workarounds.h
)

set(LIBINSANE_GOBJECT_SRCS libinsane-gobject/src/libinsane.c)
set(LIBINSANE_GOBJECT_HEADERS libinsane-gobject/include/libinsane.h)

if(WIN32)

    MESSAGE(STATUS "Target OS: Microsoft Windows (Mingw)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_WINDOWS -DCOMPILER_GCC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DOS_WINDOWS -DCOMPILER_GCC")
    set(LIBINSANE_SRCS ${LIBINSANE_SRCS}
        src/bases/twain.c
        src/bases/wia_automation.c
        src/bases/wia_ll.c
    )
    set(LIBINSANE_HEADERS ${LIBINSANE_HEADERS}
        include/twain.h
        include/wia_ll.h
        include/wia_automation.h
    )

else()

    MESSAGE(STATUS "Target OS: Unix-like")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX -DCOMPILER_GCC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX -DCOMPILER_GCC")
    set(LIBINSANE_SRCS ${LIBINSANE_SRCS}
        src/bases/sane.c
    )
    set(LIBINSANE_HEADERS ${LIBINSANE_HEADERS}
        include/sane.h
    )
    include(Sane)

endif()

# libinsane: dynamic
add_library(libinsane_dynamic SHARED ${LIBINSANE_SRCS})
target_include_directories(libinsane_dynamic PUBLIC include)
set_target_properties(libinsane_dynamic PROPERTIES OUTPUT_NAME insane)
INSTALL(TARGETS libinsane_dynamic
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)
if (DEFINED(SANE_INCLUDED_DIR))
    target_include_directories(libinsane_dynamic SYSTEM PUBLIC ${SANE_INCLUDE_DIR})
    target_link_libraries(libinsane_dynamic ${SANE_LIBRARY})
endif()


# libinsane: static
add_library(libinsane_static STATIC ${LIBINSANE_SRCS})
target_include_directories(libinsane_static PUBLIC include)
set_target_properties(libinsane_static PROPERTIES OUTPUT_NAME insane)
INSTALL(TARGETS libinsane_static
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)
if (DEFINED(SANE_INCLUDED_DIR))
    target_include_directories(libinsane_static SYSTEM PUBLIC ${SANE_INCLUDE_DIR})
    target_link_libraries(libinsane_static ${SANE_LIBRARY})
endif()
