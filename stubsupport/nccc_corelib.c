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
regcheck(int r){
    if(r < 0){
        abort();
    }
    if(r >= NCCC_DISPATCH_STACK_DEPTH){
        abort();
    }
}
static void
nccc_dispatch_N(const nccv64* op, nccv64* reg){
    const nccv64* ip = op;
    nccv64* mem;
    int src, srcb, offs;
    int dest;
    void (*func)(const nccv64* in, nccv64* out);
    for(;;){
        switch(ip[0].s32){
            case NCCC_DISPATCH_OP_RETURN:
                return;
            case NCCC_DISPATCH_OP_REGADDR: /* [1 rBASE rOUT] */
                src = ip[1].s32;
                dest = ip[2].s32;
                regcheck(src);
                regcheck(dest);
                reg[dest].ptr = &reg[src];
                ip = &ip[3];
                break;
            case NCCC_DISPATCH_OP_ADD:     /* [2 rIN1 rIN2 rOUT] */
                src = ip[1].s32;
                srcb = ip[2].s32;
                dest = ip[3].s32;
                regcheck(src);
                regcheck(srcb);
                regcheck(dest);
                /* Use unsigned for wrapping addition */
                reg[dest].uptr = reg[src].uptr + reg[srcb].uptr;
                ip = &ip[4];
                break;
            case NCCC_DISPATCH_OP_LDC:     /* [3 const rOUT] */
                dest = ip[2].s32;
                reg[dest] = ip[1];
                regcheck(dest);
                ip = &ip[3];
                break;
            case NCCC_DISPATCH_OP_LD: /* [4 rBASE offs rDEST] */
                src = ip[1].s32;
                offs = ip[2].s32;
                dest = ip[3].s32;
                mem = reg[src].ptr;
                reg[dest] = mem[offs];
                ip = &ip[4];
                break;
            case NCCC_DISPATCH_OP_ST: /* [5 rBASE offs rIN] */
                dest = ip[1].s32;
                offs = ip[2].s32;
                src = ip[3].s32;
                mem = reg[src].ptr;
                mem[offs] = reg[src];
                ip = &ip[4];
                break;
            case NCCC_DISPATCH_OP_MOV:     /* [6 rFROM rTO] */
                src = ip[1].s32;
                dest = ip[2].s32;
                regcheck(src);
                regcheck(dest);
                reg[dest] = reg[src];
                ip = &ip[3];
                break;
            case NCCC_DISPATCH_OP_CALL:    /* [7 rFUNC rINBUF rOUTBUF] */
                src = ip[1].s32;
                srcb = ip[2].s32;
                dest = ip[3].s32;
                regcheck(src);
                regcheck(srcb);
                regcheck(dest);
                func = reg[src].ptr;
                func(reg[srcb].ptr, reg[dest].ptr);
                ip = &ip[4];
                break;
            case NCCC_DISPATCH_OP_JMP: /* [8 imm] */
                offs = ip[1].s32;
                ip = &ip[offs];
            case NCCC_DISPATCH_OP_JNZ: /* [9 rIN imm] */
                src = ip[1].s32;
                offs = ip[2].s32;
                if(reg[src].s32 != 0){
                    ip = &ip[offs];
                }else{
                    ip = &ip[3];
                }
            default:
                abort();
                break;
        }
    }
}

/* Exports */
NCCC_EXPORT void
nccc_dispatch_0(const nccv64* op){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    nccc_dispatch_N(op, p);
}

NCCC_EXPORT void 
nccc_dispatch_1(const nccv64* op, void* p0){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    p[0].ptr = p0;
    nccc_dispatch_N(op, p);
}

NCCC_EXPORT void 
nccc_dispatch_2(const nccv64* op, void* p0, void* p1){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    p[0].ptr = p0;
    p[1].ptr = p1;
    nccc_dispatch_N(op, p);
}

NCCC_EXPORT void 
nccc_dispatch_3(const nccv64* op, void* p0, void* p1, void* p2){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    p[0].ptr = p0;
    p[1].ptr = p1;
    p[2].ptr = p2;
    nccc_dispatch_N(op, p);
}

NCCC_EXPORT void 
nccc_dispatch_4(const nccv64* op, void* p0, void* p1, void* p2, void* p3){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    p[0].ptr = p0;
    p[1].ptr = p1;
    p[2].ptr = p2;
    p[3].ptr = p3;
    nccc_dispatch_N(op, p);
}

NCCC_EXPORT void 
nccc_dispatch_5(const nccv64* op, void* p0, void* p1, void* p2, void* p3, 
                void* p4){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    p[0].ptr = p0;
    p[1].ptr = p1;
    p[2].ptr = p2;
    p[3].ptr = p3;
    p[4].ptr = p4;
    nccc_dispatch_N(op, p);
}

NCCC_EXPORT void 
nccc_dispatch_6(const nccv64* op, void* p0, void* p1, void* p2, void* p3, 
                void* p4, void* p5){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    p[0].ptr = p0;
    p[1].ptr = p1;
    p[2].ptr = p2;
    p[3].ptr = p3;
    p[4].ptr = p4;
    p[5].ptr = p5;
    nccc_dispatch_N(op, p);
}

NCCC_EXPORT void 
nccc_dispatch_7(const nccv64* op, void* p0, void* p1, void* p2, void* p3, 
                void* p4, void* p5, void* p6){
    nccv64 p[NCCC_DISPATCH_STACK_DEPTH];
    p[0].ptr = p0;
    p[1].ptr = p1;
    p[2].ptr = p2;
    p[3].ptr = p3;
    p[4].ptr = p4;
    p[5].ptr = p5;
    p[6].ptr = p6;
    nccc_dispatch_N(op, p);
}

