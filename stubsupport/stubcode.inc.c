/* Stubs */

#define STUBNAME_STR STUBNAME_STR0(STUBNAME, STUBNAME_STR1)
#define STUBNAME_STR0(x,y) y(x)
#define STUBNAME_STR1(x) #x
#define STUBFUNC(x) STUBFUNC0(STUBNAME,_ ## x,STUBFUNC1)
#define STUBFUNC0(x,y,z)  z(x,y)
#define STUBFUNC1(x,y) lib_ ## x ## y

typedef void* ptr;
typedef uint64_t u64;
typedef int64_t s64;
typedef uint32_t u32;
typedef int32_t s32;
typedef float f32;
typedef double f64;

#define EXP_COMMA_arg ,
#define EXP_COMMA_term

#define STUB_EMIT_ARGS(sym) \
    ( SYM_ ## sym ## _ARG_EXPAND(ITR_EMIT_ARGS) )

#define ITR_EMIT_ARGS(idx, pos, _) \
    arg ## idx EXP_COMMA_ ## pos

#define ITR_DECL_ARGS(idx,_,typ) \
    const typ arg ## idx = *(typ *)&in[idx];

#define STUB_RET_TYPE0(_,__,t) t
#define STUB_RET_TYPE(sym) SYM_ ## sym ## _RET_EXPAND(STUB_RET_TYPE0)

#define STUB_F0_void_void(sym) \
    void stub_ ## sym(const uint64_t* in, uint64_t* out){ \
        (void)in; \
        (void)out; \
        sym(); \
    }

#define STUB_F0_arg_void(sym) \
    void stub_ ## sym(const uint64_t* in, uint64_t* out){ \
        SYM_ ## sym ## _ARG_EXPAND(ITR_DECL_ARGS) \
        (void)out; \
        sym STUB_EMIT_ARGS(sym); \
    }

#define STUB_F0_void_ret(sym) \
    void stub_ ## sym(const uint64_t* in, uint64_t* out){ \
        *(STUB_RET_TYPE(sym) *)out = sym(); \
    }

#define STUB_F0_arg_ret(sym) \
    void stub_ ## sym(const uint64_t* in, uint64_t* out){ \
        SYM_ ## sym ## _ARG_EXPAND(ITR_DECL_ARGS) \
        *(STUB_RET_TYPE(sym) *)out = sym STUB_EMIT_ARGS(sym); \
    }

#define ITR_F0_STUBS(x,y,sym) \
    STUB_F0_ ## x ## _ ## y(sym)

FORWARD_0_STUB_EXPAND(ITR_F0_STUBS)

static void 
STUBFUNC(library_info)(const uint64_t* in, uint64_t* out){
    out[0] = (uintptr_t) STUBNAME_STR;
    out[1] = 0;
    out[2] = FORWARD_0_STUB_COUNT;
    out[3] = 0;
    out[4] = 0;
    out[5] = 0;
}

#define ITR_EXPORT_INFO_F0(x,y,sym) \
    case SYM_ ## sym ## _IDX: \
      out[0] = 0; \
      out[1] = SYM_ ## sym ## _IDX; \
      out[2] = (uintptr_t)#sym; \
      out[3] = 0; /* FORWARD_0 */ \
      out[4] = 0; /* Direct call */ \
      out[5] = (uintptr_t)stub_ ## sym; \
      out[6] = SYM_ ## sym ## _ARG_COUNT; \
      out[7] = SYM_ ## sym ## _RET_COUNT; \
      return;

static void
STUBFUNC(library_export_info)(const uint64_t* in, uint64_t* out){
    switch(in[0] /* fnid */ ){
        FORWARD_0_STUB_EXPAND(ITR_EXPORT_INFO_F0)
        default:
            out[0] = -1;
            return;
    }
}

#define CITYPE_VALUE_u32 0
#define CITYPE_VALUE_u64 1
#define CITYPE_VALUE_f32 2
#define CITYPE_VALUE_f64 3
#define CITYPE_VALUE_s32 4
#define CITYPE_VALUE_s64 5
#define CITYPE_VALUE_ptr 6

#define ITR_ARG_INFO_F0_ARG(offs,_,typ) \
    out[3+offs] = CITYPE_VALUE_ ## typ;

#define ITR_ARG_INFO_F0_RET(offs,_,typ) \
    out[3+offs+outoffs] = CITYPE_VALUE_ ## typ;

#define ITR_ARG_INFO_F0(_,__,sym) \
    case SYM_ ## sym ## _IDX : \
      outoffs = SYM_ ## sym ## _ARG_COUNT; \
      out[0] = 0; \
      out[1] = SYM_ ## sym ## _ARG_COUNT; \
      out[2] = SYM_ ## sym ## _RET_COUNT; \
      SYM_ ## sym ## _ARG_EXPAND(ITR_ARG_INFO_F0_ARG) \
      SYM_ ## sym ## _RET_EXPAND(ITR_ARG_INFO_F0_RET) \
      return;

static void
STUBFUNC(library_arg_info)(const uint64_t* in, uint64_t* out){
    size_t outoffs;
    switch(in[0] /* objid */){
        FORWARD_0_STUB_EXPAND(ITR_ARG_INFO_F0)
        default:
            out[0] = -1;
            return;
    }
}

void
STUBFUNC(dispatch)(const uint64_t* in, uint64_t* out){
    switch(in[0]){
        case 1:
            STUBFUNC(library_info)(NULL, out);
            return;
        case 2:
            STUBFUNC(library_export_info)(&in[1], out);
            return;
        case 6:
            STUBFUNC(library_arg_info)(&in[1], out);
            return;
        default:
            abort();
    }
}
