set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRCS
    "${SRC_ROOT}/redisclient/*.h"
    "${SRC_ROOT}/redisclient/*.hpp"
    "${SRC_ROOT}/redisclient/*.cc"
    "${SRC_ROOT}/redisclient/*.cpp"
    )

add_library(redisclient INTERFACE)
target_include_directories(redisclient
  INTERFACE
    $<BUILD_INTERFACE:${SRC_ROOT}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

auto_source_group(redisclient ${SRC_ROOT} ${SRCS})
ijk_add_executable(sync_get_set "redis" ${SRC_ROOT}/samples/sync_get_set.cc ${SRCS})
target_link_libraries(sync_get_set redisclient)
