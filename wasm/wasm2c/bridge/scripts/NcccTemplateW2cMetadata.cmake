#
# INPUTs:
#   IN_HDR: full path to header metadata (NcccParseW2cHeader)
#   IN_SYM: full path to symbol information (NcccW2cDataGen)
#   IN_SRC: full path to source metadata (NcccParseW2cSource)
#      OUT: Full path to .h
#

if(NOT IN_HDR)
    message(FATAL_ERROR "Huh?")
endif()

if(NOT IN_SYM)
    message(FATAL_ERROR "Huh?")
endif()

if(NOT OUT)
    message(FATAL_ERROR "Huh?")
endif()

set(trueout "${OUT}")
set(OUT "${OUT}.temp")

include(${IN_HDR})
include(${IN_SYM})
include(${IN_SRC})

file(WRITE ${OUT} "/* Autogenerated */\n\n")

# Total information
file(APPEND ${OUT} "#define TOTAL_EXPORTS ${symtotal_exports}\n")
file(APPEND ${OUT} "#define TOTAL_IMPORTS ${symtotal_imports}\n")
file(APPEND ${OUT} "#define TOTAL_TYPES ${typetotal_count}\n")

set(exports)
set(imports)
set(export_funcs)
set(import_funcs)
set(export_vars)
set(import_vars)

foreach(s ${syms})
    set(symtype ${sym_${s}_symtype})
    if("${symtype}" STREQUAL EXPORT_FUNCTION)
        list(APPEND exports ${s})
        list(APPEND export_funcs ${s})
    elseif("${symtype}" STREQUAL EXPORT_VARIABLE)
        list(APPEND exports ${s})
        list(APPEND export_vars ${s})
    elseif("${symtype}" STREQUAL IMPORT_FUNCTION)
        list(APPEND imports ${s})
        list(APPEND import_funcs ${s})
    elseif("${symtype}" STREQUAL IMPORT_VARIABLE)
        list(APPEND imports ${s})
        list(APPEND import_vars ${s})
    else()
        message(FATAL_ERROR "Unknown type: ${s}")
    endif()
endforeach()

# Expander macro
function(emit_funccall sym)
    set(outtype val)
    set(intype arg)
    if(${sym_${sym}_argcount} EQUAL 0)
        set(intype void)
    endif()
    if(${sym_${sym}_datatype} STREQUAL void)
        set(outtype void)
    endif()
    file(APPEND ${OUT} "    x(${intype},${outtype},${s}) \\\n")
endfunction()

function(emit_var s)
    set(vartype val)
    if(${sym_${s}_datatype} STREQUAL wasm_rt_memory_t)
        set(vartype memory)
    elseif(${sym_${s}_datatype} STREQUAL wasm_rt_table_t)
        set(vartype table)
    endif()
    file(APPEND ${OUT} "    x(${vartype},${s}) \\\n")
endfunction()

file(APPEND ${OUT} "#define EXPORT_EXPAND(x) \\\n")
foreach(s ${exports})
    file(APPEND ${OUT} "    x(${s}) \\\n")
endforeach()
file(APPEND ${OUT} "\n")
file(APPEND ${OUT} "#define IMPORT_EXPAND(x) \\\n")
foreach(s ${imports})
    file(APPEND ${OUT} "    x(${s}) \\\n")
endforeach()
file(APPEND ${OUT} "\n")
file(APPEND ${OUT} "#define IMPORTFUNC_EXPAND(x) \\\n")
foreach(s ${import_funcs})
    emit_funccall(${s})
endforeach()
file(APPEND ${OUT} "\n")
file(APPEND ${OUT} "#define IMPORTVAR_EXPAND(x) \\\n")
foreach(s ${import_vars})
    emit_var(${s})
endforeach()
file(APPEND ${OUT} "\n")
file(APPEND ${OUT} "#define EXPORTFUNC_EXPAND(x) \\\n")
foreach(s ${export_funcs})
    emit_funccall(${s})
endforeach()
file(APPEND ${OUT} "\n")
file(APPEND ${OUT} "#define EXPORTVAR_EXPAND(x) \\\n")
foreach(s ${export_vars})
    emit_var(${s})
endforeach()
file(APPEND ${OUT} "\n")

function(emit_arg_expander sym)
    set(idx 0)
    set(argcount ${sym_${sym}_argcount})
    set(type ${sym_${sym}_datatype})
    if("${type}" STREQUAL void)
        set(retcount 0)
    else()
        set(retcount 1)
    endif()
    file(APPEND ${OUT} "#define SYM_${sym}_ARG_COUNT ${argcount}\n")
    file(APPEND ${OUT} "#define SYM_${sym}_RET_COUNT ${retcount}\n")
    file(APPEND ${OUT} "#define SYM_${sym}_ARG_EXPAND(x) \\\n")
    while(NOT ${idx} EQUAL ${argcount})
        set(here ${idx})
        math(EXPR idx "${idx}+1")
        if(${idx} EQUAL ${argcount})
            set(pos term)
        else()
            set(pos arg)
        endif()
        file(APPEND ${OUT} "    x(${here},${pos},${sym_${sym}_argtype${here}}) \\\n")
    endwhile()
    file(APPEND ${OUT} "\n")
endfunction()

# Export symbol info
foreach(s ${exports})
    set(nam ${sym_${s}_exportname})
    set(idx ${sym_${s}_exportidx})
    set(type ${sym_${s}_datatype})
    file(APPEND ${OUT} "#define SYM_${s}_EXPORTNAME \"${nam}\"\n")
    file(APPEND ${OUT} "#define SYM_${s}_EXPORTIDX ${idx}\n")
    file(APPEND ${OUT} "#define SYM_${s}_DATATYPE ${type}\n")
    if("${sym_${s}_argcount}" STREQUAL "")
        file(APPEND ${OUT} "#define SYM_${s}_IS_VARIABLE 1\n")
    else()
        file(APPEND ${OUT} "#define SYM_${s}_IS_VARIABLE 0\n")
        emit_arg_expander(${s})
    endif()
endforeach()

# Import symbol info
foreach(s ${imports})
    set(nam1 ${sym_${s}_importname1})
    set(nam2 ${sym_${s}_importname2})
    set(idx ${sym_${s}_importidx})
    set(type ${sym_${s}_datatype})
    file(APPEND ${OUT} "#define SYM_${s}_IMPORTNAME1 \"${nam1}\"\n")
    file(APPEND ${OUT} "#define SYM_${s}_IMPORTNAME2 \"${nam2}\"\n")
    file(APPEND ${OUT} "#define SYM_${s}_IMPORTIDX ${idx}\n")
    file(APPEND ${OUT} "#define SYM_${s}_DATATYPE ${type}\n")
    if("${sym_${s}_argcount}" STREQUAL "")
        file(APPEND ${OUT} "#define SYM_${s}_IS_VARIABLE 1\n")
    else()
        file(APPEND ${OUT} "#define SYM_${s}_IS_VARIABLE 0\n")
        emit_arg_expander(${s})
    endif()
endforeach()

# Type info
file(APPEND ${OUT} "#define TYPE_EXPAND(x) \\\n")
foreach(t ${typenames})
    set(incount ${type_${t}_incount})
    set(outcount ${type_${t}_outcount})
    if(${incount} EQUAL 0)
        set(inmode void)
    else()
        set(inmode arg)
    endif()
    if(${outcount} EQUAL 0)
        set(outmode void)
    else()
        set(outmode val)
    endif()
    file(APPEND ${OUT} "    x(${inmode},${outmode},${t}) \\\n")
endforeach()

file(APPEND ${OUT} "\n\n")
foreach(t ${typenames})
    set(intypes ${type_${t}_intypes})
    set(outtypes ${type_${t}_outtypes})
    set(idx ${type_${t}_idx})
    set(incount ${type_${t}_incount})
    set(outcount ${type_${t}_outcount})
    #file(APPEND ${OUT} "#define TYPE_${t}_NAME \"${t}\"\n")
    file(APPEND ${OUT} "#define TYPE_${t}_INCOUNT ${incount}\n")
    file(APPEND ${OUT} "#define TYPE_${t}_OUTCOUNT ${outcount}\n")
    file(APPEND ${OUT} "#define TYPE_${t}_IDX ${idx}\n")
    if(${outcount} EQUAL 0)
        file(APPEND ${OUT} "#define TYPE_${t}_DATATYPE void\n")
    elseif(${outcount} EQUAL 1)
        file(APPEND ${OUT} "#define TYPE_${t}_DATATYPE ${outtypes}\n")
    else()
        message(FATAL_ERROR "Invalid outcount ${t}")
    endif()
    file(APPEND ${OUT} "#define TYPE_${t}_ARG_EXPAND(x) \\\n")
    set(cnt 0)
    foreach(a ${intypes})
        math(EXPR n "${cnt}+1")
        if(${incount} EQUAL ${n})
            set(mode term)
        else()
            set(mode arg)
        endif()
        file(APPEND ${OUT} "    x(${cnt},${mode},${a}) \\\n")
        math(EXPR cnt "${cnt}+1")
    endforeach()
    file(APPEND ${OUT} "\n")
endforeach()

file(RENAME ${OUT} ${trueout})
