find_package(Catch2 CONFIG REQUIRED)

set(TESTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB TESTS_SRCS
    "${TESTS_ROOT}/*.h"
    "${TESTS_ROOT}/*.hpp"
    "${TESTS_ROOT}/*.cc"
    "${TESTS_ROOT}/*.cpp"
    )

message(STATUS ${CATCH2_})

ijk_add_executable(ijk-tests "unittests" ${TESTS_SRCS})
target_link_libraries(ijk-tests ijk Catch2::Catch2)
