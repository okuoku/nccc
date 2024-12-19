cmake_minimum_required(VERSION 3.13)
project(node-nccc C)

set(common ../../common)
set(napi-sdk ../../../ext/nccc/napi-sdkfiles)

include_directories(${napi-sdk}/include ${common}
    ../../stubsupport)

add_definitions(-DNAPI_VERSION=4)

if(WIN32)
    link_directories(${napi-sdk}/lib-win64)
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

# Link settings
if(WIN32)
    target_link_libraries(node-nccc node)
elseif(APPLE)
    # Ignore any undefined
    target_link_options(node-nccc PRIVATE
        -Wl,-undefined,dynamic_lookup)
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

