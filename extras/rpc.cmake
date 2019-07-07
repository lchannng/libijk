
set(IJK_RPC_ROOT ${CMAKE_CURRENT_LIST_DIR}/rpc)

find_package(Protobuf REQUIRED)

file(GLOB_RECURSE IJK_RPC_SRCS
    "${IJK_RPC_ROOT}/ijk/*.h"
    "${IJK_RPC_ROOT}/ijk/*.hpp"
    "${IJK_RPC_ROOT}/ijk/*.cc"
    "${IJK_RPC_ROOT}/ijk/*.cpp"
    )

auto_source_group(ijk ${IJK_RPC_ROOT}/ijk ${IJK_RPC_SRCS})

add_library(ijk_rpc ${IJK_RPC_SRCS})
target_link_libraries(ijk_rpc ijk ${Protobuf_LIBRARIES})
target_include_directories(ijk_rpc PRIVATE
    ${Protobuf_INCLUDE_DIRS}
)

file(GLOB ECHO_SRCS
    "${IJK_RPC_ROOT}/samples/*.h"
    "${IJK_RPC_ROOT}/samples/*.hpp"
    "${IJK_RPC_ROOT}/samples/*.cc"
    "${IJK_RPC_ROOT}/samples/*.cpp"
    )

auto_source_group(samples ${IJK_RPC_ROOT}/samples ${ECHO_SRCS})

add_executable(echo_client ${ECHO_SRCS})
target_link_libraries(echo_client ijk ijk_rpc ${Protobuf_LIBRARIES})
target_include_directories(echo_client PRIVATE
    ${Protobuf_INCLUDE_DIRS}
    ${IJK_RPC_ROOT}
)
