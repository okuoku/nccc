#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "nccc_corelib.h"

#define STUBNAME nccc_corelib
#include "stubdata_nccc_corelib.h"
#include "stubcode.inc.c"

void
nccc_memcpy(void* to, const void* from, uintptr_t size){
    (void)memcpy(to, from, size);
}

/* Dispatch OP */
static void
nccc_dispatch_N(const nccv64* op, nccv64* out, int n, void** p){
    uintptr_t reg;
    nccv64* src;
    nccv64* tgt;
    const nccv64* ip = op;
    void (*func)(const nccv64* in, nccv64* out);
    for(;;){
        switch(ip[0].s32){
            case NCCC_DISPATCH_OP_RETURN:
                return;
            case NCCC_DISPATCH_OP_MOV:
                src = ip[1].ptr;
                tgt = ip[2].ptr;
                *tgt = *src;
                ip = &ip[3];
                break;
            case NCCC_DISPATCH_OP_MOVPTR:
                reg = ip[1].uptr;
                tgt = ip[2].ptr;
                if(reg >= n){
                    abort();
                }
                tgt[0].ptr = p[reg];
                ip = &ip[3];
                break;
            case NCCC_DISPATCH_OP_CALL:
                func = ip[1].ptr;
                src = ip[2].ptr;
                tgt = ip[3].ptr;
                func(src, tgt);
                ip = &ip[4];
                break;
            case NCCC_DISPATCH_OP_JMP:
                ip = ip[1].ptr;
                break;
            case NCCC_DISPATCH_OP_JNZ:
                reg = ip[1].uptr;
                if(reg == 0){
                    ip = &ip[3];
                }else{
                    ip = ip[2].ptr;
                }
                break;
            default:
                abort();
                break;
        }
    }
}


/* Exports */
NCCC_EXPORT void
nccc_dispatch_0(const nccv64* op, nccv64* out){
    nccc_dispatch_N(op, out, 0, 0);
}

NCCC_EXPORT void 
nccc_dispatch_1(const nccv64* op, nccv64* out, void* p0){
    void* p[1];
    p[0] = p0;
    nccc_dispatch_N(op, out, 1, p);
}

NCCC_EXPORT void 
nccc_dispatch_2(const nccv64* op, nccv64* out, void* p0, void* p1){
    void* p[2];
    p[0] = p0;
    p[1] = p1;
    nccc_dispatch_N(op, out, 2, p);
}

NCCC_EXPORT void 
nccc_dispatch_3(const nccv64* op, nccv64* out, void* p0, void* p1, void* p2){
    void* p[3];
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    nccc_dispatch_N(op, out, 3, p);
}

NCCC_EXPORT void 
nccc_dispatch_4(const nccv64* op, nccv64* out, void* p0, void* p1, void* p2, 
                void* p3){
    void* p[4];
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
    nccc_dispatch_N(op, out, 4, p);
}

NCCC_EXPORT void 
nccc_dispatch_5(const nccv64* op, nccv64* out, void* p0, void* p1, void* p2, 
                void* p3, void* p4){
    void* p[5];
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
    p[4] = p4;
    nccc_dispatch_N(op, out, 5, p);
}

NCCC_EXPORT void 
nccc_dispatch_6(const nccv64* op, nccv64* out, void* p0, void* p1, void* p2, 
                void* p3, void* p4, void* p5){
    void* p[6];
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
    p[4] = p4;
    p[5] = p5;
    nccc_dispatch_N(op, out, 6, p);
}

NCCC_EXPORT void 
nccc_dispatch_7(const nccv64* op, nccv64* out, void* p0, void* p1, void* p2, 
                void* p3, void* p4, void* p5, void* p6){
    void* p[7];
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
    p[4] = p4;
    p[5] = p5;
    p[6] = p6;
    nccc_dispatch_N(op, out, 7, p);
}

