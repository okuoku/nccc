#include <stdint.h>
#include <stddef.h>

#if _MSC_VER
#include <intrin.h>
#define ABORT __debugbreak
#else
#define ABORT __builtin_trap
#endif

typedef void (*nccc_call_t)(const uint64_t* in, uint64_t* out);

#define EXP_COMMA_arg ,
#define EXP_COMMA_term

#define EXP_DECL_ARGS(sym) \
    ( SYM_ ## sym ## _ARG_EXPAND(EXP__ARGS) )

#define EXP__ARGS(idx, pos, typ) \
    typ arg ## idx EXP_COMMA_ ## pos

/* EXPORT FUNCs */

#define EXP_DECL_EXPORT_FUNC(in,out,sym) \
    EXP_DECL_EXPORT_FUNC_ ## in ## _ ## out (sym)

#define EXP_DECL_INARGS_ITR(idx,_,typ) \
    const typ arg ## idx = *(typ *)&in[idx];

#define EXP_DECL_INARGS(sym) \
    SYM_ ## sym ## _ARG_EXPAND(EXP_DECL_INARGS_ITR)

#define EXP_CALLARGS_ITR(idx,pos,_) \
    arg ## idx EXP_COMMA_ ## pos

#define EXP_CALLARGS(sym) \
    ( SYM_ ## sym ## _ARG_EXPAND(EXP_CALLARGS_ITR) )

#define EXP_DECL_EXPORT_FUNC_arg_val(sym) \
    static void nccc_ ## sym (const uint64_t* in, uint64_t* out) { \
        SYM_ ## sym ## _DATATYPE ret; \
        EXP_DECL_INARGS(sym) \
        ret = WASM_RT_ADD_PREFIX(sym) \
        EXP_CALLARGS(sym); \
        *((SYM_ ## sym ## _DATATYPE *)out) = ret; \
    } \
    extern SYM_ ## sym ## _DATATYPE \
    (*WASM_RT_ADD_PREFIX(sym)) EXP_DECL_ARGS(sym); \

#define EXP_DECL_EXPORT_FUNC_void_val(sym) \
    static void nccc_ ## sym (const uint64_t* in, uint64_t* out) { \
        SYM_ ## sym ## _DATATYPE ret; \
        ret = WASM_RT_ADD_PREFIX(sym)(); \
        *((SYM_ ## sym ## _DATATYPE *)out) = ret; \
    } \
    extern SYM_ ## sym ## _DATATYPE \
    (*WASM_RT_ADD_PREFIX(sym)) EXP_DECL_ARGS(sym);

#define EXP_DECL_EXPORT_FUNC_arg_void(sym) \
    static void nccc_ ## sym (const uint64_t* in, uint64_t* out) { \
        EXP_DECL_INARGS(sym) \
        WASM_RT_ADD_PREFIX(sym) \
        EXP_CALLARGS(sym); \
    } \
    extern SYM_ ## sym ## _DATATYPE \
    (*WASM_RT_ADD_PREFIX(sym)) EXP_DECL_ARGS(sym);

#define EXP_DECL_EXPORT_FUNC_void_void(sym) \
    static void nccc_ ## sym (const uint64_t* in, uint64_t* out) { \
        WASM_RT_ADD_PREFIX(sym)(); \
    } \
    extern SYM_ ## sym ## _DATATYPE \
    (*WASM_RT_ADD_PREFIX(sym)) EXP_DECL_ARGS(sym);

/* Function types (implicit exports) */

#define EXP_BRIDGETYPE(sym) functype_ ## sym ## _t
#define EXP_DECL_BRIDGE_ARGS_ITR(_,pos,typ) \
    typ EXP_COMMA_ ## pos
#define EXP_DECL_BRIDGE_ARGS(sym) \
    ( TYPE_ ## sym ## _ARG_EXPAND(EXP_DECL_BRIDGE_ARGS_ITR) )
#define EXP_DECL_BRIDGETYPE(sym) \
    typedef TYPE_ ## sym ## _DATATYPE \
    (*EXP_BRIDGETYPE(sym)) EXP_DECL_BRIDGE_ARGS(sym)
#define EXP_DECL_BRIDGETYPE_void(sym) \
    typedef TYPE_ ## sym ## _DATATYPE \
    (*EXP_BRIDGETYPE(sym))(void)

#define EXP_DECL_BRIDGE_INARGS_ITR(idx,_,typ) \
    const typ arg ## idx = *(typ *)&in[idx+1];
#define EXP_DECL_BRIDGE_INARGS(sym) \
    TYPE_ ## sym ## _ARG_EXPAND(EXP_DECL_BRIDGE_INARGS_ITR)

#define EXP_BRIDGE_CALLARGS_ITR(idx,pos,_) \
    arg ## idx EXP_COMMA_ ## pos
#define EXP_BRIDGE_CALLARGS(sym) \
    ( TYPE_ ## sym ## _ARG_EXPAND(EXP_BRIDGE_CALLARGS_ITR) )

#define EXP_DECL_TYPE_BRIDGE_arg_val(sym) \
    EXP_DECL_BRIDGETYPE(sym); \
    static void \
    typestub_ ## sym (const uint64_t* in, uint64_t* out) { \
        const EXP_BRIDGETYPE(sym) func = \
        (EXP_BRIDGETYPE(sym))(intptr_t)in[0]; \
        EXP_DECL_BRIDGE_INARGS(sym) \
        TYPE_ ## sym ## _DATATYPE ret; \
        ret = func \
        EXP_BRIDGE_CALLARGS(sym) ; \
        *((TYPE_ ## sym ## _DATATYPE *)out) = ret; \
    }

#define EXP_DECL_TYPE_BRIDGE_arg_void(sym) \
    EXP_DECL_BRIDGETYPE(sym); \
    static void \
    typestub_ ## sym (const uint64_t* in, uint64_t* out) { \
        const EXP_BRIDGETYPE(sym) func = \
        (EXP_BRIDGETYPE(sym))(intptr_t)in[0]; \
        EXP_DECL_BRIDGE_INARGS(sym) \
        func \
        EXP_BRIDGE_CALLARGS(sym) ; \
    }

#define EXP_DECL_TYPE_BRIDGE_void_val(sym) \
    EXP_DECL_BRIDGETYPE_void(sym); \
    static void \
    typestub_ ## sym (const uint64_t* in, uint64_t* out) { \
        const EXP_BRIDGETYPE(sym) func = \
        (EXP_BRIDGETYPE(sym))(intptr_t)in[0]; \
        TYPE_ ## sym ## _DATATYPE ret; \
        ret = func(); \
        *((TYPE_ ## sym ## _DATATYPE *)out) = ret; \
    }

#define EXP_DECL_TYPE_BRIDGE_void_void(sym) \
    EXP_DECL_BRIDGETYPE_void(sym); \
    static void \
    typestub_ ## sym (const uint64_t* in, uint64_t* out) { \
        EXP_BRIDGETYPE(sym) func = \
        (EXP_BRIDGETYPE(sym))(intptr_t)in[0]; \
        func(); \
    }

#define EXP_DECL_TYPE_BRIDGE(in,out,sym) \
    EXP_DECL_TYPE_BRIDGE_ ## in ## _ ## out (sym)

/* EXPORT VARIABLEs */
#define EXP_DECL_EXPORT_VAR_obj(sym) \
    extern SYM_ ## sym ## _DATATYPE * WASM_RT_ADD_PREFIX(sym);
#define EXP_DECL_EXPORT_VAR_table(sym) \
    EXP_DECL_EXPORT_VAR_obj(sym)
#define EXP_DECL_EXPORT_VAR_memory(sym) \
    EXP_DECL_EXPORT_VAR_obj(sym)
#define EXP_DECL_EXPORT_VAR(type,sym) \
    EXP_DECL_EXPORT_VAR_ ## type(sym)

/* IMPORT FUNCs */

#define EXP_DECL_IMPORT_FUNC(in, out, sym) \
    EXP_DECL_IMPORT_FUNC_ ## in ## _ ## out (sym)

#define EXP_DECL_INBUF_BODY_arg(_)
#define EXP_DECL_INBUF_BODY_term(idx) uint64_t in[idx+1]
#define EXP_DECL_INBUF_ITR(idx, pos, type) \
    EXP_DECL_INBUF_BODY_ ## pos (idx)
#define EXP_DECL_INBUF(sym) \
    SYM_ ## sym ## _ARG_EXPAND(EXP_DECL_INBUF_ITR)

#define EXP_ASSIGN_INBUF_ITR(idx,_,__) \
    in[idx] = *(uint64_t*)&arg ## idx;

#define EXP_ASSIGN_INBUF(sym) \
    SYM_ ## sym ## _ARG_EXPAND(EXP_ASSIGN_INBUF_ITR)

#define EXP_DECL_IMPORT_FUNC_arg_val(sym) \
    static nccc_call_t ncccdispatch_ ## sym; \
    static uintptr_t nccc_ ## sym; \
    static SYM_ ## sym ## _DATATYPE \
    instub_ ## sym EXP_DECL_ARGS(sym) { \
        uint64_t callargs[2]; \
        uint64_t out; \
        EXP_DECL_INBUF(sym); \
        EXP_ASSIGN_INBUF(sym); \
        callargs[0] = (uintptr_t)nccc_ ## sym; \
        callargs[1] = (uintptr_t)in; \
        ncccdispatch_ ## sym(callargs, &out); \
        return *(SYM_ ## sym ## _DATATYPE *)&out; \
    } \
    SYM_ ## sym ## _DATATYPE (*sym) EXP_DECL_ARGS(sym) = \
    instub_ ## sym; 
    

#define EXP_DECL_IMPORT_FUNC_void_val(sym) \
    static nccc_call_t ncccdispatch_ ## sym; \
    static uintptr_t nccc_ ## sym; \
    static SYM_ ## sym ## _DATATYPE \
    instub_ ## sym (void) { \
        uint64_t callargs[2]; \
        uint64_t out; \
        callargs[0] = (uintptr_t)nccc_ ## sym; \
        callargs[1] = 0; \
        ncccdispatch_ ## sym(callargs, &out); \
        return *(SYM_ ## sym ## _DATATYPE *)&out; \
    } \
    SYM_ ## sym ## _DATATYPE (*sym) EXP_DECL_ARGS(sym) = \
    instub_ ## sym;

#define EXP_DECL_IMPORT_FUNC_arg_void(sym) \
    static nccc_call_t ncccdispatch_ ## sym; \
    static uintptr_t nccc_ ## sym; \
    static void \
    instub_ ## sym EXP_DECL_ARGS(sym) { \
        uint64_t callargs[2]; \
        EXP_DECL_INBUF(sym); \
        EXP_ASSIGN_INBUF(sym); \
        callargs[0] = nccc_ ## sym; \
        callargs[1] = (uintptr_t)in; \
        ncccdispatch_ ## sym(callargs, NULL); \
    } \
    void (*sym) EXP_DECL_ARGS(sym) = \
    instub_ ## sym;

#define EXP_DECL_IMPORT_FUNC_void_void(sym) \
    static nccc_call_t ncccdispatch_ ## sym; \
    static uintptr_t nccc_ ## sym; \
    static void \
    instub_ ## sym (void) { \
        uint64_t callargs[2]; \
        callargs[0] = (uintptr_t)nccc_ ## sym; \
        callargs[1] = 0; \
        ncccdispatch_ ## sym(callargs, NULL); \
    } \
    void (*sym) (void) = \
    instub_ ## sym;

/* IMPORT VARIABLEs */
#define EXP_DECL_IMPORT_VAR_val(sym) \
    SYM_ ## sym ## _DATATYPE stubstore_ ## sym; \
    SYM_ ## sym ## _DATATYPE* sym = & stubstore_ ## sym;

#define EXP_DECL_IMPORT_VAR_obj(sym) \
    SYM_ ## sym ## _DATATYPE* sym = 0;

#define EXP_DECL_IMPORT_VAR_memory(sym) \
    EXP_DECL_IMPORT_VAR_obj(sym)

#define EXP_DECL_IMPORT_VAR_table(sym) \
    EXP_DECL_IMPORT_VAR_obj(sym)

#define EXP_DECL_IMPORT_VAR(type,sym) \
    EXP_DECL_IMPORT_VAR_ ## type(sym)


IMPORTVAR_EXPAND(EXP_DECL_IMPORT_VAR)
IMPORTFUNC_EXPAND(EXP_DECL_IMPORT_FUNC)
EXPORTFUNC_EXPAND(EXP_DECL_EXPORT_FUNC)
EXPORTVAR_EXPAND(EXP_DECL_EXPORT_VAR) 
TYPE_EXPAND(EXP_DECL_TYPE_BRIDGE)

uint32_t wasm_rt_call_stack_depth;

void
stub_wasm_library_info(const uint64_t* in, uint64_t* out){
    const uint64_t library_index = in[0];
    const uint64_t export_count = TOTAL_EXPORTS;
    const uint64_t import_count = TOTAL_IMPORTS;
    const uint64_t callinfo_count = TOTAL_EXPORTS + TOTAL_IMPORTS;
    const uint64_t type_count = TOTAL_TYPES;

    if(library_index != 0){
        ABORT();
    }

    out[0] = export_count;
    out[1] = import_count;
    out[2] = callinfo_count;
    out[3] = type_count;
}

#define TOTALINDEX_EXPORT(sym) (SYM_ ## sym ## _EXPORTIDX + TOTAL_IMPORTS)
#define LIBEX(sym) \
    case SYM_ ## sym ## _EXPORTIDX: \
      res = 0; \
      name = SYM_ ## sym ## _EXPORTNAME; \
      is_variable = SYM_ ## sym ## _IS_VARIABLE; \
      callinfoidx = TOTALINDEX_EXPORT(sym); \
      break; 

#define LIBEX_FUNC(_,__,sym) \
    case SYM_ ## sym ## _EXPORTIDX: \
      value = (uintptr_t)nccc_ ## sym; \
      break; 

#define LIBEX_VAR(_,sym) \
    case SYM_ ## sym ## _EXPORTIDX: \
      value = (uintptr_t)&WASM_RT_ADD_PREFIX(sym); \
      break; 

void
stub_library_get_export(const uint64_t* in, uint64_t* out){
    const uint64_t idx = in[0];
    const char* name = NULL;
    int is_variable = 0;
    uint64_t res;
    uint64_t value;
    uint64_t callinfoidx;
    switch(idx){
        EXPORT_EXPAND(LIBEX)
        default:
            res = -1;
            value = -1;
            callinfoidx = -1;
            break;
    }
    if(is_variable){
        switch(idx){
            EXPORTVAR_EXPAND(LIBEX_VAR)
            default:
                ABORT();
                break;
        }
    }else{
        switch(idx){
            EXPORTFUNC_EXPAND(LIBEX_FUNC)
            default:
                ABORT();
                break;
        }
    }
    out[0] = res;
    out[1] = (uintptr_t)name;
    out[2] = value;
    out[3] = callinfoidx;
    out[4] = is_variable;
}

#define TOTALINDEX_IMPORT(sym) (SYM_ ## sym ## _IMPORTIDX)
#define LIBIM_FUNC(_,__,sym) \
    case SYM_ ## sym ## _IMPORTIDX: \
      res = 0; \
      name0 = SYM_ ## sym ## _IMPORTNAME1; \
      name1 = SYM_ ## sym ## _IMPORTNAME2; \
      callinfoidx = TOTALINDEX_IMPORT(sym); \
      is_variable = 0; \
      break;

#define LIBIM_VAR(_,sym) \
    case SYM_ ## sym ## _IMPORTIDX: \
      res = 0; \
      name0 = SYM_ ## sym ## _IMPORTNAME1; \
      name1 = SYM_ ## sym ## _IMPORTNAME2; \
      callinfoidx = TOTALINDEX_IMPORT(sym); \
      is_variable = 1; \
      break;

void
stub_library_get_import(const uint64_t* in, uint64_t* out){
    const uint64_t idx = in[0];
    const char* name0 = NULL;
    const char* name1 = NULL;
    uint64_t callinfoidx;
    uint64_t res;
    int is_variable = 0;
    switch(idx){
        IMPORTVAR_EXPAND(LIBIM_VAR)
        IMPORTFUNC_EXPAND(LIBIM_FUNC)
        default:
            res = -1;
            callinfoidx = -1;
            break;
    }
    out[0] = res;
    out[1] = (uintptr_t)name0;
    out[2] = (uintptr_t)name1;
    out[3] = callinfoidx;
    out[4] = is_variable;
}

#define LIBIMSETVAR_table(sym) \
    LIBIMSETVAR_obj(sym)
#define LIBIMSETVAR_memory(sym) \
    LIBIMSETVAR_obj(sym)
#define LIBIMSETVAR_obj(sym) \
    case SYM_ ## sym ## _IMPORTIDX: \
      res = 0; \
      sym = (SYM_ ## sym ## _DATATYPE *)(uintptr_t)value; \
      break;
#define LIBIMSETVAR_val(sym) \
    case SYM_ ## sym ## _IMPORTIDX: \
      res = 0; \
      stubstore_ ## sym = *(SYM_ ## sym ## _DATATYPE *)&value; \
      break;

#define LIBIMSETVAR(type,sym) \
    LIBIMSETVAR_ ## type(sym)

#define LIBIMSETFUNC(_,__,sym) \
    case SYM_ ## sym ## _IMPORTIDX: \
      res = 0; \
      ncccdispatch_ ## sym = (nccc_call_t)dispatch; \
      nccc_ ## sym = (uintptr_t)value; \
      break;

void
stub_library_set_import(const uint64_t* in, uint64_t* out){
    const uint64_t idx = in[0];
    const uint64_t dispatch = in[1];
    const uint64_t value = in[2];
    uint64_t res;
    switch(idx){
        IMPORTVAR_EXPAND(LIBIMSETVAR)
        IMPORTFUNC_EXPAND(LIBIMSETFUNC)
        default:
            res = -1;
            break;
    }
    out[0] = res;
}

#define CIIM_COUNT(_,__,sym) \
    case TOTALINDEX_IMPORT(sym): \
      res = 0; \
      incount = SYM_ ## sym ## _ARG_COUNT; \
      outcount = SYM_ ## sym ## _RET_COUNT; \
      break;

#define CIIM_COUNT_VAR(_,sym) \
    case TOTALINDEX_IMPORT(sym): \
      res = 0; \
      incount = 0; \
      outcount = 1; \
      break;

#define CIEX_COUNT_FUNC(_,__,sym) \
    case TOTALINDEX_EXPORT(sym): \
      res = 0; \
      incount = SYM_ ## sym ## _ARG_COUNT; \
      outcount = SYM_ ## sym ## _RET_COUNT; \
      break;

#define CIEX_COUNT_VAR(_,sym) \
    case TOTALINDEX_EXPORT(sym): \
      res = 0; \
      incount = 0; \
      outcount = 1; \
      break;

void
stub_callinfo_get_counts(const uint64_t* in, uint64_t* out){
    const uint64_t idx = in[0];
    uint64_t res;
    uint64_t incount;
    uint64_t outcount;
    switch(idx){
        IMPORTFUNC_EXPAND(CIIM_COUNT)
        IMPORTVAR_EXPAND(CIIM_COUNT_VAR)
        EXPORTFUNC_EXPAND(CIEX_COUNT_FUNC)
        EXPORTVAR_EXPAND(CIEX_COUNT_VAR)
        default:
            res = -1;
            incount = 0;
            outcount = 0;
            break;
    }
    out[0] = res;
    out[1] = incount;
    out[2] = outcount;
}

#define CIEXP(x,y) y(x)
#define CITYPE(x) CITYPE_VALUE_ ## x
#define CITYPE_VALUE_u32 0
#define CITYPE_VALUE_u64 1
#define CITYPE_VALUE_f32 2
#define CITYPE_VALUE_f64 3
#define CITYPE_VALUE_wasm_rt_memory_t -1
#define CITYPE_VALUE_wasm_rt_table_t -2

#define CI_TYPES_SET(idx,_,type) \
    out[3+idx] = CITYPE(type);

#define CIFILLRET_void(sym)

#define CIFILLRET_val(sym) \
    out[3+SYM_ ## sym ## _ARG_COUNT] = CIEXP(SYM_ ## sym ## _DATATYPE,CITYPE); 

#define CIIM_TYPES_FUNC(_,ret,sym) \
    case TOTALINDEX_IMPORT(sym): \
      res = 0; \
      out[1] = SYM_ ## sym ## _ARG_COUNT; \
      out[2] = SYM_ ## sym ## _RET_COUNT; \
      SYM_ ## sym ## _ARG_EXPAND(CI_TYPES_SET) \
      CIFILLRET_ ## ret(sym) \
      break;

#define CIIM_TYPES_VAR(_,sym) \
    case TOTALINDEX_IMPORT(sym): \
      res = 0; \
      out[1] = 0; \
      out[2] = 1; \
      out[3] = CIEXP(SYM_ ## sym ## _DATATYPE,CITYPE); \
      break;

#define CIEX_TYPES_FUNC(_,ret,sym) \
    case TOTALINDEX_EXPORT(sym): \
      res = 0; \
      out[1] = SYM_ ## sym ## _ARG_COUNT; \
      out[2] = SYM_ ## sym ## _RET_COUNT; \
      SYM_ ## sym ## _ARG_EXPAND(CI_TYPES_SET) \
      CIFILLRET_ ## ret(sym) \
      break;

#define CIEX_TYPES_VAR(_,sym) \
    case TOTALINDEX_EXPORT(sym): \
      res = 0; \
      out[1] = 0; \
      out[2] = 1; \
      out[3] = CIEXP(SYM_ ## sym ## _DATATYPE,CITYPE); \
      break;

void
stub_callinfo_get_types(const uint64_t* in, uint64_t* out){
    const uint64_t idx = in[0];
    uint64_t res;
    switch(idx){
        IMPORTFUNC_EXPAND(CIIM_TYPES_FUNC)
        IMPORTVAR_EXPAND(CIIM_TYPES_VAR)
        EXPORTFUNC_EXPAND(CIEX_TYPES_FUNC)
        EXPORTVAR_EXPAND(CIEX_TYPES_VAR)
        default:
            res = -1;
            break;
    }
    out[0] = res;
}

#define TBGC_ITR(_,__,sym) \
    case TYPE_ ## sym ## _IDX: \
      res = 0; \
      argcount = TYPE_ ## sym ## _INCOUNT; \
      retcount = TYPE_ ## sym ## _OUTCOUNT; \
      break;

void
stub_typebridge_get_counts(const uint64_t* in, uint64_t* out){
    const uint64_t idx = in[0];
    uint64_t res;
    uint64_t argcount;
    uint64_t retcount;

    switch(idx){
        TYPE_EXPAND(TBGC_ITR)
        default:
            res = -1;
            argcount = 0;
            retcount = 0;
            break;
    }

    out[0] = res;
    out[1] = argcount;
    out[2] = retcount;
}

#define TGBT_RET_void(sym)
#define TGBT_RET_val(sym) \
    out[4 + TYPE_ ## sym ## _INCOUNT] = CIEXP(TYPE_ ## sym ## _DATATYPE,CITYPE);

#define TGBT_ARG_ITR(pos,_,type) \
      out[4+pos] = CITYPE(type);

#define TBGT_ITR(_,ret,sym) \
    case TYPE_ ## sym ## _IDX: \
      res = 0; \
      out[1] = (uintptr_t)typestub_ ## sym; \
      out[2] = TYPE_ ## sym ## _INCOUNT; \
      out[3] = TYPE_ ## sym ## _OUTCOUNT; \
      TYPE_ ## sym ## _ARG_EXPAND(TGBT_ARG_ITR) \
      TGBT_RET_ ## ret(sym) \
      break;

void
stub_typebridge_get_types(const uint64_t* in, uint64_t* out){
    const uint64_t idx = in[0];
    uint64_t res;
    switch(idx){
        TYPE_EXPAND(TBGT_ITR)
        default:
            res = -1;
            break;
    }
    out[0] = res;
}
