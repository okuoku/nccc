cmake_minimum_required(VERSION 3.12)
project(ncccstubsupport C)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/NcccStubMacros.cmake)

# Generate stub desc

nccc_stub_begin(nccc_corelib)
nccc_api(nccc_memcpy IN ptr ptr uptr OUT)
nccc_stub_end(nccc_corelib)

if(YFRM_BUILD_MODULE)
    add_definitions(-DNCCC_DLL)
    set(libtype MODULE)
else()
    set(libtype STATIC)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(nccc_nccc_corelib ${libtype}
    nccc_corelib.c
    )

