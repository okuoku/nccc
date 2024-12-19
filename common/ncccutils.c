#include <stdint.h>
#define UTILLIB_API

#include <stdlib.h>

#include "nccv64.h"

typedef void (*nccc_call_t)(const nccv64* in, nccv64* out);

UTILLIB_API void
util_rawcall(const nccv64* in, nccv64* out){
    // [addr inbuf outbuf] => []
    const nccc_call_t addr = (nccc_call_t)in[0].ptr;
    const nccv64* inbuf = in[1].ptr;
    nccv64* const outbuf = in[2].ptr;

    addr(inbuf,outbuf);
}

UTILLIB_API void
util_peek_u64(const nccv64* in, nccv64* out){
    // [addr] => [v]
    const uint64_t* addr = in[0].ptr;
    const uint64_t d = *addr;
    out[0].u64 = d;
}

UTILLIB_API void
util_peek_u32(const nccv64* in, nccv64* out){
    // [addr] => [v]
    const uint32_t* addr = in[0].ptr;
    const uint32_t d = *addr;
    out[0].u32 = d;
}

UTILLIB_API void
util_peek_u8(const nccv64* in, nccv64* out){
    // [addr] => [v]
    const uint8_t* addr = in[0].ptr;
    const uint8_t d = *addr;
    out[0].u8 = d;
}

UTILLIB_API void
util_peek_ptr(const nccv64* in, nccv64* out){
    // [addr] => [v]
    const uintptr_t* addr = in[0].ptr;
    const uintptr_t d = *addr;
    out[0].uptr = d;
}

UTILLIB_API void
util_poke_u64(const nccv64* in, nccv64* out){
    // [addr v] => []
    uint64_t* const dest = in[0].ptr;
    (void) out;
    *dest = in[1].u64;
}

UTILLIB_API void
util_poke_u32(const nccv64* in, nccv64* out){
    // [addr v] => []
    uint32_t* const dest = in[0].ptr;
    (void) out;
    *dest = in[1].u32;
}

UTILLIB_API void
util_malloc(const nccv64* in, nccv64* out){
    // [size] => [addr]
    const size_t size = in[0].u64;
    void* addr = malloc(size);
    if(!addr){
        abort();
    }
    out[0].ptr = addr;
}

UTILLIB_API void
util_free(const nccv64* in, nccv64* out){
    // [addr] => []
    void* addr = in[0].ptr;
    free(addr);
}

UTILLIB_API void
util_ptraddr(const nccv64* in, nccv64* out){
    // [addr] => [addr]
    out[0].u64 = in[0].u64;
}

UTILLIB_API void
util_shortcircuit(const nccv64* in, nccv64* out){
    // Short-circuit call for wasm stub
    // [nccc-func-addr in] => dispatch
    const nccc_call_t addr = (nccc_call_t)in[0].ptr;
    const nccv64* inbuf = in[1].ptr;

    addr(inbuf,out);
}

/* FIXME: Move these to elsewhere */
#include <math.h>

UTILLIB_API void
math_cos(const nccv64* in, nccv64* out){
    float x;
    float r;
    x = in[0].f32;
    r = cosf(x);
    out[0].f32 = r;
}
UTILLIB_API void
math_sin(const nccv64* in, nccv64* out){
    float x;
    float r;
    x = in[0].f32;
    r = sinf(x);
    out[0].f32 = r;
}
UTILLIB_API void
math_atan2(const nccv64* in, nccv64* out){
    float x;
    float y;
    float r;
    y = in[0].f32;
    x = in[1].f32;
    r = atan2f(y,x);
    out[0].f32 = r;
}
UTILLIB_API void
math_sqrt(const nccv64* in, nccv64* out){
    float x;
    float r;
    x = in[0].f32;
    r = sqrtf(x);
    out[0].f32 = r;
}
UTILLIB_API void
math_pow(const nccv64* in, nccv64* out){
    float b;
    float e;
    float r;
    b = in[0].f32;
    e = in[1].f32;
    r = powf(b,e);
    out[0].f32 = r;
}
UTILLIB_API void
math_fmodf(const nccv64* in, nccv64* out){
    // FIXME: https://cpprefjp.github.io/reference/cmath/fmod.html
    float x;
    float y;
    float r;
    x = in[0].f32;
    y = in[1].f32;
    r = remainder(fabs(x), (y = fabs(y)));
    if (signbit(r)) {
        r += y;
    }
    r = copysign(r, x);
    out[0].f32 = r;
}
