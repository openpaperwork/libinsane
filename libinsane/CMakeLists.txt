cmake_minimum_required(VERSION 3.5)

project(Libinsane)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wunused -pedantic -g -ggdb -O2 -fstack-protector-all")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wunused -pedantic -g -ggdb -O2 -fstack-protector-all")

set(LIBINSANE_SRCS
    src/bases/dumb.c
    src/error.c
    src/log.c
    src/multiplexer.c
    src/normalizers/all_opts_on_all_sources.c
    src/normalizers/end_of_feed.c
    src/normalizers/flatbed_and_feeder_behavior.c
    src/normalizers/min_one_source.c
    src/normalizers/raw.c
    src/normalizers/raw24.c
    src/normalizers/resolution_format.c
    src/normalizers/scan_area_opts.c
    src/normalizers/source_names.c
    src/normalizers/source_nodes.c
    src/normalizers/source_types.c
    src/normalizers/sources_order.c
    src/normalizers/strip_non_scanners.c
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
    include/libinsane/capi.h
    include/libinsane/dumb.h
    include/libinsane/error.h
    include/libinsane/log.h
    include/libinsane/multiplexer.h
    include/libinsane/normalizers.h
    include/libinsane/safebet.h
    include/libinsane/str2impls.h
    include/libinsane/util.h
    include/libinsane/workarounds.h
)

# Some tests can safely go through valgrinf
# Some other will never pass (libsane and its drivers have leaks for instance)

set(LIBINSANE_TESTS
    sane
)
set(LIBINSANE_VALGRIND_TESTS
    multiplexer
)

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
        include/libinsane/twain.h
        include/libinsane/wia_ll.h
        include/libinsane/wia_automation.h
    )

    set(LIBINSANE_INCLUDES)
    set(LIBINSANE_LIBS)

else()

    MESSAGE(STATUS "Target OS: Unix-like")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX -DCOMPILER_GCC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX -DCOMPILER_GCC")
    set(LIBINSANE_SRCS ${LIBINSANE_SRCS}
        src/bases/sane.c
    )
    set(LIBINSANE_HEADERS ${LIBINSANE_HEADERS}
        include/libinsane/sane.h
    )
    include(Sane)
    set(LIBINSANE_INCLUDES ${SANE_INCLUDE_DIR})
    set(LIBINSANE_LIBS ${SANE_LIBRARY})

endif()

install(FILES
    ${LIBINSANE_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libinsane)

# libinsane: dynamic
add_library(libinsane_dynamic SHARED ${LIBINSANE_SRCS})
target_include_directories(libinsane_dynamic PUBLIC include ${LIBINSANE_INCLUDES})
target_link_libraries(libinsane_dynamic ${LIBINSANE_LIBS})
set_target_properties(libinsane_dynamic PROPERTIES OUTPUT_NAME insane)
INSTALL(TARGETS libinsane_dynamic
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
if (DEFINED(SANE_INCLUDED_DIR))
    target_include_directories(libinsane_dynamic SYSTEM PUBLIC ${SANE_INCLUDE_DIR})
    target_link_libraries(libinsane_dynamic ${SANE_LIBRARY})
endif()


# libinsane: static
add_library(libinsane_static STATIC ${LIBINSANE_SRCS})
target_include_directories(libinsane_static PUBLIC include ${LIBINSANE_INCLUDES})
target_link_libraries(libinsane_static ${LIBINSANE_LIBS})
set_target_properties(libinsane_static PROPERTIES OUTPUT_NAME insane)
INSTALL(TARGETS libinsane_static
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
if (DEFINED(SANE_INCLUDED_DIR))
    target_include_directories(libinsane_static SYSTEM PUBLIC ${SANE_INCLUDE_DIR})
    target_link_libraries(libinsane_static ${SANE_LIBRARY})
endif()


# libinsane tests
enable_testing()

find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes" "--leak-check=full" "--error-exitcode=10")
set(ENV{CTEST_OUTPUT_ON_FAILURE} "1")
MESSAGE(STATUS "Valgrind: ${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS}")

foreach(test ${LIBINSANE_TESTS})
    add_executable(tests_${test} ${LIBINSANE_SRCS} tests/main.c tests/tests_${test}.c)
    target_include_directories(tests_${test} PUBLIC include ${LIBINSANE_INCLUDES})
    target_link_libraries(tests_${test} ${LIBINSANE_LIBS} -lcunit)
    add_test(
        NAME tests_${test}
        COMMAND ${CMAKE_BINARY_DIR}/tests_${test}
    )
endforeach(test)
foreach(test ${LIBINSANE_VALGRIND_TESTS})
    add_executable(tests_${test} ${LIBINSANE_SRCS} tests/main.c tests/tests_${test}.c)
    target_include_directories(tests_${test} PUBLIC include ${LIBINSANE_INCLUDES})
    target_link_libraries(tests_${test} ${LIBINSANE_LIBS} -lcunit)
    add_test(
        NAME tests_${test}
        COMMAND ${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS} -- ${CMAKE_BINARY_DIR}/tests_${test}
    )
endforeach(test)
