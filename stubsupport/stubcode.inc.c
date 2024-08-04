/* Stubs */

#ifdef NCCC_DLL
#ifdef _WIN32
#define NCCC_EXPORT __declspec(dllexport)
#else
#define NCCC_EXPORT __attribute__ ((visibility ("default")))
#endif
#else /* !NCCC_DLL = Generic static-library */
#ifdef _WIN32
#define NCCC_EXPORT
#else
#define NCCC_EXPORT __attribute__ ((visibility ("default")))
#endif
#endif

#define STUBNAME_STR STUBNAME_STR0(STUBNAME, STUBNAME_STR1)
#define STUBNAME_STR0(x,y) y(x)
#define STUBNAME_STR1(x) #x
#define STUBFUNC(x) STUBFUNC0(STUBNAME,_ ## x,STUBFUNC1)
#define STUBFUNC0(x,y,z)  z(x,y)
#define STUBFUNC1(x,y) lib_ ## x ## y ## _ncccv0

#include <stdint.h>
#include <stdlib.h> /* Abort */

union nccv64_u {
    void* ptr;
    uintptr_t uptr;
    intptr_t sptr;
    uint64_t u64;
    int64_t s64;
    uint32_t u32;
    int32_t s32;
    uint16_t u16;
    int16_t s16;
    uint8_t u8;
    int8_t s8;
    float f32;
    double f64;
};

typedef union nccv64_u nccv64;

#define EXP_COMMA_arg ,
#define EXP_COMMA_term

#define STUB_EMIT_ARGS(sym) \
    ( SYM_ ## sym ## _ARG_EXPAND(ITR_EMIT_ARGS) )

#define ITR_EMIT_ARGS(idx, pos, typ) \
    in[idx].typ EXP_COMMA_ ## pos

#define STUB_RET_TYPE0(_,__,t) t
#define STUB_RET_TYPE(sym) SYM_ ## sym ## _RET_EXPAND(STUB_RET_TYPE0)

#define STUB_F0_void_void(sym) \
    void stub_ ## sym(const nccv64* in, nccv64* out){ \
        (void)in; \
        (void)out; \
        sym(); \
    }

#define STUB_F0_arg_void(sym) \
    void stub_ ## sym(const nccv64* in, nccv64* out){ \
        (void)out; \
        sym STUB_EMIT_ARGS(sym); \
    }

#define STUB_F0_void_ret(sym) \
    void stub_ ## sym(const nccv64* in, nccv64* out){ \
        out->STUB_RET_TYPE(sym) = sym(); \
    }

#define STUB_F0_arg_ret(sym) \
    void stub_ ## sym(const nccv64* in, nccv64* out){ \
        out->STUB_RET_TYPE(sym) = sym STUB_EMIT_ARGS(sym); \
    }

#define ITR_F0_STUBS(x,y,sym) \
    STUB_F0_ ## x ## _ ## y(sym)

FORWARD_0_STUB_EXPAND(ITR_F0_STUBS)

static void 
STUBFUNC(library_info)(const nccv64* in, nccv64* out){
    out[0].ptr = STUBNAME_STR;
    out[1].u64 = 0;
    out[2].s32 = FORWARD_0_STUB_COUNT;
    out[3].u64 = 0;
    out[4].u64 = 0;
    out[5].u64 = 0;
}

#define ITR_EXPORT_INFO_F0(x,y,sym) \
    case SYM_ ## sym ## _IDX: \
      out[0].s32 = 0; \
      out[1].s32 = SYM_ ## sym ## _IDX; \
      out[2].ptr = #sym; \
      out[3].s32 = 0; /* FORWARD_0 */ \
      out[4].s32 = 0; /* Direct call */ \
      out[5].ptr = stub_ ## sym; \
      out[6].s32 = SYM_ ## sym ## _ARG_COUNT; \
      out[7].s32 = SYM_ ## sym ## _RET_COUNT; \
      return;

static void
STUBFUNC(library_export_info)(const nccv64* in, nccv64* out){
    switch(in[0].s32 /* fnid */ ){
        FORWARD_0_STUB_EXPAND(ITR_EXPORT_INFO_F0)
        default:
            out[0].s32 = -1;
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
#define CITYPE_VALUE_uptr 7
#define CITYPE_VALUE_sptr 8
#define CITYPE_VALUE_u8 9
#define CITYPE_VALUE_u16 10
#define CITYPE_VALUE_s8 11
#define CITYPE_VALUE_s16 12

#define ITR_ARG_INFO_F0_ARG(offs,_,typ) \
    out[3+offs].s32 = CITYPE_VALUE_ ## typ;

#define ITR_ARG_INFO_F0_RET(offs,_,typ) \
    out[3+offs+outoffs].s32 = CITYPE_VALUE_ ## typ;

#define ITR_ARG_INFO_F0(_,__,sym) \
    case SYM_ ## sym ## _IDX : \
      outoffs = SYM_ ## sym ## _ARG_COUNT; \
      out[0].s32 = 0; \
      out[1].s32 = SYM_ ## sym ## _ARG_COUNT; \
      out[2].s32 = SYM_ ## sym ## _RET_COUNT; \
      SYM_ ## sym ## _ARG_EXPAND(ITR_ARG_INFO_F0_ARG) \
      SYM_ ## sym ## _RET_EXPAND(ITR_ARG_INFO_F0_RET) \
      return;

static void
STUBFUNC(library_arg_info)(const nccv64* in, nccv64* out){
    size_t outoffs;
    switch(in[0].s32 /* objid */){
        FORWARD_0_STUB_EXPAND(ITR_ARG_INFO_F0)
        default:
            out->s32 = -1;
            return;
    }
}

NCCC_EXPORT void
STUBFUNC(dispatch)(const nccv64* in, nccv64* out){
    switch(in[0].s32){
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
