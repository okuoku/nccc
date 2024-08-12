#ifndef NCCC__NCCV64_H
#define NCCC__NCCV64_H

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

#endif
