#include <stdint.h>
#define UTILLIB_API

#include <stdlib.h>

typedef void (*nccc_call_t)(const uint64_t* in, uint64_t* out);

UTILLIB_API void
util_rawcall(const uint64_t* in, uint64_t* out){
    // [addr inbuf outbuf] => []
    const nccc_call_t addr = (nccc_call_t)in[0];
    const uint64_t* inbuf = (uintptr_t)in[1];
    uint64_t* const outbuf = (uintptr_t)in[2];

    addr(inbuf,outbuf);
}

UTILLIB_API void
util_peek_u64(const uint64_t* in, uint64_t* out){
    // [addr] => [v]
    const uint64_t* addr = (uintptr_t)in[0];
    const uint64_t d = *addr;
    out[0] = d;
}

UTILLIB_API void
util_peek_u32(const uint64_t* in, uint64_t* out){
    // [addr] => [v]
    const uint32_t* addr = (uintptr_t)in[0];
    const uint32_t d = *addr;
    out[0] = d;
}

UTILLIB_API void
util_peek_ptr(const uint64_t* in, uint64_t* out){
    // [addr] => [v]
    const uintptr_t* addr = (uintptr_t)in[0];
    const uint64_t d = *addr;
    out[0] = d;
}

UTILLIB_API void
util_poke_u64(const uint64_t* in, uint64_t* out){
    // [addr v] => []
    uint64_t* const dest = (uintptr_t)in[0];
    (void) out;
    *dest = in[1];
}

UTILLIB_API void
util_poke_u32(const uint64_t* in, uint64_t* out){
    // [addr v] => []
    uint32_t* const dest = (uintptr_t)in[0];
    (void) out;
    *dest = in[1];
}

UTILLIB_API void
util_malloc(const uint64_t* in, uint64_t* out){
    // [size] => [addr]
    const size_t size = in[0];
    const void* addr = malloc(size);
    if(!addr){
        abort();
    }
    out[0] = addr;
}

UTILLIB_API void
util_free(const uint64_t* in, uint64_t* out){
    // [addr] => []
    const void* addr = (void*)(uintptr_t)in[0];
    free(addr);
}

UTILLIB_API void
util_ptraddr(const uint64_t* in, uint64_t* out){
    // [addr] => [addr]
    out[0] = in[0];
}

UTILLIB_API void
util_shortcircuit(const uint64_t* in, uint64_t* out){
    // Short-circuit call for wasm stub
    // [nccc-func-addr in] => dispatch
    const nccc_call_t addr = (nccc_call_t)in[0];
    const uint64_t* inbuf = (uintptr_t)in[1];

    addr(inbuf,out);
}

/* FIXME: Move these to elsewhere */
#include <math.h>

UTILLIB_API void
math_cos(const uint64_t* in, uint64_t* out){
    float x;
    float r;
    x = *(float *)in;
    r = cosf(x);
    *(float *)out = r;
}
UTILLIB_API void
math_sin(const uint64_t* in, uint64_t* out){
    float x;
    float r;
    x = *(float *)in;
    r = sinf(x);
    *(float *)out = r;
}
UTILLIB_API void
math_atan2(const uint64_t* in, uint64_t* out){
    float x;
    float y;
    float r;
    y = *(float *)&in[0];
    x = *(float *)&in[1];
    r = atan2f(y,x);
    *(float *)out = r;
}
UTILLIB_API void
math_sqrt(const uint64_t* in, uint64_t* out){
    float x;
    float r;
    x = *(float *)in;
    r = sqrtf(x);
    *(float *)out = r;
}
UTILLIB_API void
math_pow(const uint64_t* in, uint64_t* out){
    float b;
    float e;
    float r;
    b = *(float *)&in[0];
    e = *(float *)&in[1];
    r = powf(b,e);
    *(float *)out = r;
}
UTILLIB_API void
math_fmodf(const uint64_t* in, uint64_t* out){
    // FIXME: https://cpprefjp.github.io/reference/cmath/fmod.html
    float x;
    float y;
    float r;
    x = *(float *)&in[0];
    y = *(float *)&in[1];
    r = remainder(fabs(x), (y = fabs(y)));
    if (signbit(r)) {
        r += y;
    }
    r = copysign(r, x);
    *(float *)out = r;
}
