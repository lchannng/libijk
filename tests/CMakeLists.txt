find_package(Catch2 CONFIG REQUIRED)

set(TESTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB TESTS_SRCS
    "${TESTS_ROOT}/*.h"
    "${TESTS_ROOT}/*.hpp"
    "${TESTS_ROOT}/*.cc"
    "${TESTS_ROOT}/*.cpp"
    )

auto_source_group(ijk ${IJK_SRC_ROOT} ${IJK_HDRS})

ijk_add_executable(ijk-tests "unittests" ${TESTS_SRCS} ${IJK_HDRS})
target_link_libraries(ijk-tests ijk Catch2::Catch2)
