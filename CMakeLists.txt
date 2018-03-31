cmake_minimum_required(VERSION 3.7)

project(Libinsane)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(LIBINSANE_SRCS libinsane/src/safebet.c)
set(LIBINSANE_HEADERS
    libinsane/include/capi.h
    libinsane/include/error.h
    libinsane/include/multiplexer.h
    libinsane/include/normalizers.h
    libinsane/include/util.h
    libinsane/include/workarounds.h
)

if($ENV{OS} MATCHES "linux")

    MESSAGE(STATUS "Target OS: Unix-like")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX -DCOMPILER_GCC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX -DCOMPILER_GCC")
    set(LIBINSANE_SRCS ${LIBINSANE_SRCS} libinsane/src/sane.c)
    set(LIBINSANE_HEADERS ${LIBINSANE_HEADERS} libinsane/include/sane.h)
    include(GtkDoc)
    include(Sane)

elseif($ENV{OS} MATCHES "mingw")

    MESSAGE(STATUS "Target OS: Microsoft Windows (Mingw)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_WINDOWS -DCOMPILER_GCC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DOS_WINDOWS -DCOMPILER_GCC")
    set(LIBINSANE_SRCS ${LIBINSANE_SRCS} libinsane/src/wia_ll.c libinsane/src/wia_automation.c libinsane/src/twain.c)
    set(LIBINSANE_HEADERS ${LIBINSANE_HEADERS}
        libinsane/include/twain.h
        libinsane/include/wia_ll.h
        libinsane/include/wia_automation.h
    )
    include(Mingw)

else()

    MESSAGE(FATAL_ERROR "Unknown target OS: $ENV{OS}. Valid OS are: linux|mingw. Use environment variable 'OS'.")

endif()

# dynamic
add_library(libinsane_dynamic SHARED ${LIBINSANE_SRCS})
target_include_directories(libinsane_dynamic PUBLIC libinsane/include)
set_target_properties(libinsane_dynamic PROPERTIES OUTPUT_NAME insane)
INSTALL(TARGETS libinsane_dynamic
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

# static
add_library(libinsane_static STATIC ${LIBINSANE_SRCS})
target_include_directories(libinsane_static PUBLIC libinsane/include)
set_target_properties(libinsane_static PROPERTIES OUTPUT_NAME insane)
INSTALL(TARGETS libinsane_static
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

if (DEFINED(SANE_INCLUDED_DIR))
    target_include_directories(libinsane SYSTEM PUBLIC ${SANE_INCLUDE_DIR})
    target_link_libraries(libinsane ${SANE_LIBRARY})
endif()
