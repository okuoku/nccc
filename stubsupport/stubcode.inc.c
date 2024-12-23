/* Stubs */

#define STUBNAME_STR STUBNAME_STR0(STUBNAME, STUBNAME_STR1)
#define STUBNAME_STR0(x,y) y(x)
#define STUBNAME_STR1(x) #x
#define STUBFUNC(x) STUBFUNC0(STUBNAME,_ ## x,STUBFUNC1)
#define STUBFUNC0(x,y,z)  z(x,y)
#define STUBFUNC1(x,y) lib_ ## x ## y ## _ncccv0

#include <stdint.h>
#include <stdlib.h> /* Abort */

#include "ncccexport.h"
#include "nccv64.h"

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
        (void)in; \
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
    (void)in;
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
      out[5].ptr = (void*)stub_ ## sym; \
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

#define CITYPE_VALUE_u32 0  /* I */
#define CITYPE_VALUE_u64 1  /* L */
#define CITYPE_VALUE_f32 2  /* f */
#define CITYPE_VALUE_f64 3  /* d */
#define CITYPE_VALUE_s32 4  /* i */
#define CITYPE_VALUE_s64 5  /* l */
#define CITYPE_VALUE_ptr 6  /* p */
#define CITYPE_VALUE_uptr 7 /* X */
#define CITYPE_VALUE_sptr 8 /* x */
#define CITYPE_VALUE_u8 9   /* B */
#define CITYPE_VALUE_u16 10 /* H */
#define CITYPE_VALUE_s8 11  /* b */
#define CITYPE_VALUE_s16 12 /* h */

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
