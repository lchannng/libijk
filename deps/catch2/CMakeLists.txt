
add_library(catch2 INTERFACE)

if (USE_EXTERNAL_CATCH2)
    find_package(Catch2 CONFIG REQUIRED)
    target_link_libraries(catch2 INTERFACE Catch2::Catch2)
else()
    set(CATCH2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/catch2-2.10.2")

    file(GLOB_RECURSE CATCH2_HDRS
        "${CATCH2_ROOT}/*.h"
        "${CATCH2_ROOT}/*.hpp"
        "${CATCH2_ROOT}/*.ipp"
        )


    target_include_directories(catch2
        INTERFACE
        ${CATCH2_ROOT}
        )
endif()
