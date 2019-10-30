add_library(asio INTERFACE)

if (USE_BOOST_ASIO)
    find_package(Boost REQUIRED COMPONENTS system)
    add_definitions(-DBOOST_ASIO_NO_DEPRECATED)

    target_link_libraries(asio
        INTERFACE
        ${Boost_LIBRARIES}
        )

    target_include_directories(asio
        INTERFACE
        ${Boost_INCLUDE_DIRS}
        )

    target_compile_definitions(asio
        INTERFACE
        -DBOOST_ASIO_NO_DEPRECATED
        )
else()
    set(ASIO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/asio-1.13.0")

    file(GLOB_RECURSE ASIO_HDRS
        "${ASIO_ROOT}/*.h"
        "${ASIO_ROOT}/*.hpp"
        "${ASIO_ROOT}/*.ipp"
        )

    target_include_directories(asio
        INTERFACE
        ${ASIO_ROOT}
        )

    target_compile_definitions(asio
        INTERFACE
        -DASIO_STANDALONE
        -DASIO_NO_DEPRECATED
        )

    auto_install_files("" "${ASIO_ROOT}" ${ASIO_HDRS})
endif()
