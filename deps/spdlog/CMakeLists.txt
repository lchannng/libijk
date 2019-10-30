
set(SPDLOG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/spdlog-1.3.1/include")

file(GLOB_RECURSE SPDLOG_HDRS
    "${SPDLOG_ROOT}/*.h"
    "${SPDLOG_ROOT}/*.hpp"
    "${SPDLOG_ROOT}/*.ipp"
    )

add_library(spdlog INTERFACE)

target_include_directories(spdlog
    INTERFACE
    ${SPDLOG_ROOT}
    )

auto_install_files("" "${SPDLOG_ROOT}" ${SPDLOG_HDRS})

