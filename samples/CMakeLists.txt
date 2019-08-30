set(SAMPLES_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

macro(add_sample target_name)
    ijk_add_executable(${target_name} "samples" ${ARGN})
    target_link_libraries(${target_name} ijk)
endmacro(add_sample name)

add_sample(sample01 ${SAMPLES_ROOT}/sample01.cc)
add_sample(sample02 ${SAMPLES_ROOT}/sample02.cc)
add_sample(benchmark ${SAMPLES_ROOT}/benchmark.cc)
add_sample(future ${SAMPLES_ROOT}/future.cc)

