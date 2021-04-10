cmake_minimum_required(VERSION 3.0)
project(node-nccc C)

set(common ../../common)
set(napi-sdk ../../../napi-sdkfiles)

link_directories(${napi-sdk}/lib-win64)
include_directories(${napi-sdk}/include ${common})

if(WIN32)
    set(dlfcn
        ${common}/win32-dlfcn.h
        ${common}/win32-dlfcn.c)
else()
    set(dlfcn)
endif()


add_library(node-nccc SHARED
    node-nccc.c
    ${common}/ncccutils.c
    ${common}/ncccdlfcn.c
    ${dlfcn}
    )

if(WIN32)
    target_link_libraries(node-nccc node)
endif()

set_target_properties(node-nccc
    PROPERTIES
    PREFIX ""
    SUFFIX ".node")

if(NODENCCC_DEBUG_COPY)
    add_custom_command(TARGET node-nccc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:node-nccc>"
        "${NODENCCC_DEBUG_COPY}")
endif()

