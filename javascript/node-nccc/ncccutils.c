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
