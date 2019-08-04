find_package(Catch2 CONFIG REQUIRED)

set(TESTS_ROOT ${CMAKE_CURRENT_LIST_DIR})

file(GLOB TESTS_SRCS
    "${TESTS_ROOT}/*.h"
    "${TESTS_ROOT}/*.hpp"
    "${TESTS_ROOT}/*.cc"
    "${TESTS_ROOT}/*.cpp"
    )

message(STATUS ${CATCH2_})

add_executable(ijk-tests ${TESTS_SRCS} ${IJK_SOURCE_FILES})
target_link_libraries(ijk-tests ijk Catch2::Catch2)
