set(SAMPLES_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sample01 ${SAMPLES_ROOT}/sample01.cc)
target_link_libraries(sample01 ijk)

add_executable(sample02 ${SAMPLES_ROOT}/sample02.cc)
target_link_libraries(sample02 ijk)
