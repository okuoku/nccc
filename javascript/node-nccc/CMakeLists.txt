cmake_minimum_required(VERSION 3.0)
project(node-nccc C)

link_directories(napi-lib)
include_directories(napi-includes)

if(WIN32)
    set(dlfcn
        win32-dlfcn.h
        win32-dlfcn.c)
else()
    set(dlfcn)
endif()


add_library(node-nccc SHARED
    node-nccc.c
    ncccutils.c
    ncccdlfcn.c
    ${dlfcn}
    )

if(WIN32)
    target_link_libraries(node-nccc node)
endif()

set_target_properties(node-nccc
    PROPERTIES
    PREFIX ""
    SUFFIX ".node")

