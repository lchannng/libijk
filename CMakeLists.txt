project (ijk)
cmake_minimum_required (VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unittests" ON)
option(BUILD_SAMPLES "Build samples" ON)
option(BUILD_EXTRAS "Build extra libraries" OFF)

if (CMAKE_VERSION VERSION_LESS 2.8.5)
    set(CMAKE_INSTALL_BINDIR "bin" CACHE STRING "User executables (bin)")
    set(CMAKE_INSTALL_LIBDIR "lib${LIB_SUFFIX}" CACHE STRING "Object code libraries (lib)")
    set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE STRING "C header files (include)")
    mark_as_advanced(CMAKE_INSTALL_BINDIR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)
else()
    include(GNUInstallDirs)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
        CACHE STRING "Set build type to Debug o Release (default Debug)" FORCE)
endif()

#include("${CMAKE_CURRENT_SOURCE_DIR}/configure.cmake")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake")

find_package(Threads REQUIRED)

# add_definitions(-Wall)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
# Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

if(WIN32)
    macro(get_WIN32_WINNT version)
        if (CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            # Check for Windows 10, b/c we'll need to convert to hex 'A'.
            if ("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif ("${verMajor}" MATCHES "10")
            # Remove all remaining '.' characters.
            string(REPLACE "." "" ver ${ver})
            # Prepend each digit with a zero.
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif(CMAKE_SYSTEM_VERSION)
    endmacro(get_WIN32_WINNT)
    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
elseif(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_NO_DEPRECATED)

set(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/ijk")
set(ASIO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/deps/asio")
set(SPDLOG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/deps/spdlog-1.3.1/include")

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${ASIO_ROOT}
    ${SPDLOG_ROOT}
    )

file(GLOB_RECURSE IJK_HDRS
    "${SRC_ROOT}/*.h"
    "${SRC_ROOT}/*.hpp"
    "${SRC_ROOT}/*.ipp"
    )

auto_source_group(ijk ${SRC_ROOT} ${IJK_HDRS})

file(GLOB_RECURSE ASIO_HDRS
    "${ASIO_ROOT}/*.h"
    "${ASIO_ROOT}/*.hpp"
    "${ASIO_ROOT}/*.ipp"
    )

auto_source_group(asio ${ASIO_ROOT} ${ASIO_HDRS})

file(GLOB_RECURSE SPDLOG_HDRS
    "${SPDLOG_ROOT}/*.h"
    "${SPDLOG_ROOT}/*.hpp"
    "${SPDLOG_ROOT}/*.ipp"
    )

auto_source_group(spdlog ${SPDLOG_ROOT} ${SPDLOG_HDRS})

add_library(ijk INTERFACE)
target_include_directories(ijk
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (BUILD_TESTS)
    include(tests/tests.cmake)
endif()

if (BUILD_SAMPLES)
    include(samples/samples.cmake)
endif()

if (BUILD_EXTRAS)
    include(extras/rpc.cmake)
endif()

auto_install_files(ijk "${SRC_ROOT}" ${IJK_HDRS})
auto_install_files("" "${ASIO_ROOT}" ${ASIO_HDRS})
auto_install_files("" "${SPDLOG_ROOT}" ${SPDLOG_HDRS})

message(STATUS "")
message(STATUS "BUILD_TESTS:              " ${BUILD_TESTS})
message(STATUS "BUILD_SAMPLES:            " ${BUILD_SAMPLES})
message(STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "PROJECT_BINARY_DIR:       " ${PROJECT_BINARY_DIR})
message(STATUS "PROJECT_SOURCE_DIR:       " ${PROJECT_SOURCE_DIR})
message(STATUS "CMAKE_MODULE_PATH:        " ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_SYSTEM:             " ${CMAKE_SYSTEM} )
message(STATUS "CMAKE_SYSTEM_NAME:        " ${CMAKE_SYSTEM_NAME} )
message(STATUS "CMAKE_SYSTEM_VERSION:     " ${CMAKE_SYSTEM_VERSION} )
message(STATUS "CMAKE_SYSTEM_PROCESSOR:   " ${CMAKE_SYSTEM_PROCESSOR} )
message(STATUS "CMAKE_VERBOSE_MAKEFILE:   " ${CMAKE_VERBOSE_MAKEFILE} )
message(STATUS "CMAKE_CXX_FLAGS:          " ${CMAKE_CXX_FLAGS} )
message(STATUS "CMAKE_BUILD_TYPE:         " ${CMAKE_BUILD_TYPE} )
message(STATUS "CMAKE_CXX_COMPILER:       " ${CMAKE_CXX_COMPILER} )
message(STATUS "CMAKE_AR:                 " ${CMAKE_AR} )
message(STATUS "CMAKE_RANLIB:             " ${CMAKE_RANLIB} )
message(STATUS "CMAKE_INSTALL_PREFIX:     " ${CMAKE_INSTALL_PREFIX} )
message(STATUS "CMAKE_INSTALL_INCLUDEDIR: " ${CMAKE_INSTALL_INCLUDEDIR} )
message(STATUS "CMAKE_INSTALL_LIBDIR:     " ${CMAKE_INSTALL_LIBDIR} )
message(STATUS "CMAKE_INSTALL_BINDIR:     " ${CMAKE_INSTALL_BINDIR} )
message(STATUS "")

set(IJK_INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${ASIO_ROOT} ${SPDLOG_ROOT} CACHE STRING "ijk include directories")
set(IJK_LIBRARIES ijk CACHE STRING "ijk libraries")
set(IJK_SOURCE_FILES ${IJK_HDRS} ${ASIO_HDRS} ${SPDLOG_HDRS} CACHE STRING "ijk source feils")
