cmake_minimum_required(VERSION 3.20)
project(nccc-tests C)

if(NOT NCCC_ROOT)
    cmake_path(SET NCCC_ROOT NORMALIZE ${CMAKE_CURRENT_LIST_DIR}/../../..)
endif()

include(${NCCC_ROOT}/stubsupport/cmake/NcccStubMacros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stub_tests_nccc.cmake)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${NCCC_ROOT}/stubsupport)

if(BUILD_NCCC_MODULE)
    set(libtype MODULE)
else()
    set(libtype STATIC)
endif()

add_library(nccc_tests_nccc ${libtype} tests_nccc.c)

if(MSVC)
    target_compile_options(nccc_tests_nccc PRIVATE /W4)
else()
    target_compile_options(nccc_tests_nccc PRIVATE
        -Wall -Wextra -pedantic)
endif()
