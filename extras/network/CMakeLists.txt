find_package(Protobuf REQUIRED)

set(NETWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SERVER_NETWORK_SRCS
    "${NETWORK_ROOT}/src/*.h"
    "${NETWORK_ROOT}/src/*.hpp"
    "${NETWORK_ROOT}/src/*.cc"
    "${NETWORK_ROOT}/src/*.cpp"
    )

add_library(snet ${SERVER_NETWORK_SRCS})
target_link_libraries(snet ${Protobuf_LIBRARIES} ijk)
target_include_directories(snet PRIVATE
    ${NETWORK_ROOT}/src
    ${Protobuf_INCLUDE_DIRS}
)
