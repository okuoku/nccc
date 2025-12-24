cmake_minimum_required(VERSION 3.0)
project(duk-nccc C)

set(NCCC_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/../node-nccc)
set(DUK ${CMAKE_CURRENT_SOURCE_DIR}/../duktape-releases)
set(DUK_SRC ${CMAKE_CURRENT_LIST_DIR}/../duktape260)
set(DUK_EXTRAS ${DUK}/extras)

include_directories(${DUK_SRC}
    ${DUK_EXTRAS}/print-alert)

add_definitions(
    -DSCRIPTROOT=\"${CMAKE_CURRENT_LIST_DIR}\"
    #-DDUK_USE_DEBUG=1
    #-DDUK_USE_DEBUG_LEVEL=1
    #-DDUK_USE_DEBUG_WRITE=dukdebugwrite
    )

if(WIN32)
    add_definitions(
        -DDUK_USE_GET_MONOTONIC_TIME_WINDOWS_QPC=1
        )
else()
    add_definitions(
        -DDUK_USE_GET_MONOTONIC_TIME_CLOCK_GETTIME=1
        )
endif()

set(duk_srcs0
    duk_alloc_default.c      duk_debug_macros.c       duk_hthread_alloc.c
    duk_api_buffer.c         duk_debug_vsnprintf.c    duk_hthread_builtins.c
    duk_api_bytecode.c       duk_debugger.c           duk_hthread_misc.c
    duk_api_call.c           duk_debugger.h           duk_hthread_stacks.c
    duk_api_codec.c          duk_error.h              duk_internal.h
    duk_api_compile.c        duk_error_augment.c      duk_jmpbuf.h
    duk_api_debug.c          duk_error_longjmp.c      duk_js.h
    duk_api_heap.c           duk_error_macros.c       duk_js_arith.c
    duk_api_inspect.c        duk_error_misc.c         duk_js_bytecode.h
    duk_api_internal.h       duk_error_throw.c        duk_js_call.c
    duk_api_memory.c         duk_exception.h          duk_js_compiler.c
    duk_api_object.c         duk_fltunion.h           duk_js_compiler.h
    duk_api_random.c         duk_forwdecl.h           duk_js_executor.c
    duk_api_stack.c          duk_harray.h             duk_js_ops.c
    duk_api_string.c         duk_hboundfunc.h         duk_js_var.c
    duk_api_time.c           duk_hbuffer.h            duk_json.h
    duk_bi_array.c           duk_hbuffer_alloc.c      duk_lexer.c
    duk_bi_boolean.c         duk_hbuffer_assert.c     duk_lexer.h
    duk_bi_buffer.c          duk_hbuffer_ops.c        duk_numconv.c
    duk_bi_cbor.c            duk_hbufobj.h            duk_numconv.h
    duk_bi_date.c            duk_hbufobj_misc.c       duk_refcount.h
    duk_bi_date_unix.c       duk_hcompfunc.h          duk_regexp.h
    duk_bi_date_windows.c    duk_heap.h               duk_regexp_compiler.c
    duk_bi_duktape.c         duk_heap_alloc.c         duk_regexp_executor.c
    duk_bi_encoding.c        duk_heap_finalize.c      duk_replacements.c
    duk_bi_error.c           duk_heap_hashstring.c    duk_replacements.h
    duk_bi_function.c        duk_heap_markandsweep.c  duk_selftest.c
    duk_bi_global.c          duk_heap_memory.c        duk_selftest.h
    duk_bi_json.c            duk_heap_misc.c          duk_source_meta.json
    duk_bi_math.c            duk_heap_refcount.c      duk_strings.h
    duk_bi_number.c          duk_heap_stringcache.c   duk_tval.c
    duk_bi_object.c          duk_heap_stringtable.c   duk_tval.h
    duk_bi_performance.c     duk_heaphdr.h            duk_unicode.h
    duk_bi_pointer.c         duk_heaphdr_assert.c     duk_unicode_support.c
    duk_bi_promise.c         duk_henv.h               duk_unicode_tables.c
    duk_bi_protos.h          duk_hnatfunc.h           duk_util.h
    duk_bi_proxy.c           duk_hobject.h            duk_util_bitdecoder.c
    duk_bi_reflect.c         duk_hobject_alloc.c      duk_util_bitencoder.c
    duk_bi_regexp.c          duk_hobject_assert.c     duk_util_bufwriter.c
    duk_bi_string.c          duk_hobject_class.c      duk_util_cast.c
    duk_bi_symbol.c          duk_hobject_enum.c       duk_util_double.c
    duk_bi_thread.c          duk_hobject_misc.c       duk_util_hashbytes.c
    duk_bi_thrower.c         duk_hobject_pc2line.c    duk_util_memory.c
    duk_builtins.c           duk_hobject_props.c      duk_util_memrw.c
    duk_builtins.h           duk_hproxy.h             duk_util_misc.c
    duk_config.h             duk_hstring.h            duk_util_tinyrandom.c
    duk_dblunion.h           duk_hstring_assert.c     duktape.h
    duk_debug.h              duk_hstring_misc.c
    duk_debug_fixedbuffer.c  duk_hthread.h)

set(duk_srcs)

list(APPEND
    duk_srcs
    ${DUK_EXTRAS}/print-alert/duk_print_alert.c)

set(duk_use_amalgamated OFF)

if(duk_use_amalgamated)
    message(STATUS "Duktape: Amalgamated build.")
    list(APPEND duk_srcs ${DUK}/src/duktape.c)
else()
    foreach(s ${duk_srcs0})
        list(APPEND duk_srcs ${DUK_SRC}/${s})
    endforeach()
endif()


if(WIN32)
    set(dlfcn
        ${NCCC_COMMON}/win32-dlfcn.h
        ${NCCC_COMMON}/win32-dlfcn.c)
else()
    set(dlfcn)
endif()

add_executable(yuniduk
    yuniduk.c
    duk-nccc.c
    ${NCCC_COMMON}/ncccutils.c
    ${NCCC_COMMON}/ncccdlfcn.c
    ${dlfcn}
    ${duk_srcs})

if(WIN32)
else()
    target_link_libraries(yuniduk m dl)
endif()
