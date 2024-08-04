if(NOT NCCC_ROOT)
    set(NCCC_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

cmake_minimum_required(VERSION 3.20)
project(nccc-tests C)

include(${NCCC_ROOT}/stubsupport/cmake/NcccStubMacros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stub_tests_nccc.cmake)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${NCCC_ROOT}/stubsupport)

if(YFRM_BUILD_MODULE) # FIXME: NCCC..?
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
