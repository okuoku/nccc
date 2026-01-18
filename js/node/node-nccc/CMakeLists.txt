cmake_minimum_required(VERSION 3.20)
project(node-nccc C)

cmake_path(SET reporoot NORMALIZE ${CMAKE_CURRENT_LIST_DIR}/../../..)

cmake_path(SET common NORMALIZE ${reporoot}/common)
cmake_path(SET napi-sdk NORMALIZE ${reporoot}/../ext/js/node-api-headers)
cmake_path(SET stubsupport NORMALIZE ${reporoot}/stubsupport)

include_directories(${napi-sdk}/include ${common} ${stubsupport})

add_definitions(-DNAPI_VERSION=4)

function(gen_implib)
    cmake_path(SET def NORMALIZE ${napi-sdk}/lib/node_api.def)
    cmake_path(NATIVE_PATH def defN)
    if(MSVC)
        set(machine x64) # x86 x64 arm64
        cmake_path(SET implib NORMALIZE ${CMAKE_CURRENT_BINARY_DIR}/node.lib)
        cmake_path(NATIVE_PATH implib implibN)
        add_custom_command(OUTPUT ${implib}
            COMMAND lib.exe /def:${defN} /machine:${machine} /out:${implibN})
    else() 
        # MinGW(binutils)
        set(dlltool dlltool)
        cmake_path(SET implib NORMALIZE ${CMAKE_CURRENT_BINARY_DIR}/node.exe.a)
        cmake_path(NATIVE_PATH implib implibN)
        add_custom_command(OUTPUT ${implib}
            COMMAND dlltool -d ${defN} -D node.exe -l ${implibN})
    endif()

    add_library(node IMPORTED)
    set_target_properties(node PROPERTIES
        IMPORTED_IMPLIB ${implib}
        IMPORTED_LOCATION ${implib})
endfunction()

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

# Link settings
if(WIN32)
    gen_implib()
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

if(NCCC_DEBUG_NODE_MODULES)
    if(NCCC_DEBUG_MODULES_PREFIX)
        message(STATUS "prefix: ${NCCC_DEBUG_MODULES_PREFIX}")
    else()
        message(FATAL_ERROR "no modules prefix")
    endif()
    add_custom_command(TARGET node-nccc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:node-nccc>"
        "${NCCC_DEBUG_NODE_MODULES}/node-nccc.node")
    configure_file(nccc-debug-prefix.js.in
        ${NCCC_DEBUG_NODE_MODULES}/nccc-debug-prefix.js @ONLY)
endif()

