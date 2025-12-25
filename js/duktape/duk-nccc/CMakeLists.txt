cmake_minimum_required(VERSION 3.21)
project(duk-nccc C)

cmake_path(SET projroot NORMALIZE ${CMAKE_CURRENT_LIST_DIR}/../../..)
cmake_path(SET common NORMALIZE ${projroot}/common)
cmake_path(SET stubsupport NORMALIZE ${projroot}/stubsupport)

include_directories(${stubsupport})

if(WIN32)
    set(dlfcn
        ${common}/win32-dlfcn.h
        ${common}/win32-dlfcn.c)
else()
    set(dlfcn)
endif()

add_library(duk-nccc OBJECT
    duk-nccc.c
    ${common}/ncccutils.c
    ${common}/ncccdlfcn.c
    ${dlfcn})

if(WIN32)
else()
    target_link_libraries(duk-nccc m dl)
endif()
