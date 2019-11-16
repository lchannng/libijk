set(TESTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE TESTS_SRCS
    "${TESTS_ROOT}/*.h"
    "${TESTS_ROOT}/*.hpp"
    "${TESTS_ROOT}/*.cc"
    "${TESTS_ROOT}/*.cpp"
    )

auto_source_group(tests ${TESTS_ROOT} ${TESTS_SRCS})

ijk_add_executable(ijk-tests "unittests" ${TESTS_SRCS})
target_link_libraries(ijk-tests ijk catch2)
