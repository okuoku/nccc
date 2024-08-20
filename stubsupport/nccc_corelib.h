#ifndef NCCC__NCCC_CORELIB_H
#define NCCC__NCCC_CORELIB_H

#include <stdint.h>
#include "nccv64.h"
#include "ncccexport.h"

/* Memory utils */
void nccc_memcpy(void* to, const void* from, uintptr_t size);

/* Dispatch */
#define NCCC_DISPATCH_STACK_DEPTH 32
#define NCCC_DISPATCH_OP_RETURN 0
#define NCCC_DISPATCH_OP_REGADDR 1 /* [1 rBASE rOUT] */
#define NCCC_DISPATCH_OP_ADD     2 /* [2 rIN1 rIN2 rOUT] */
#define NCCC_DISPATCH_OP_LDC     3 /* [3 const rOUT] */
#define NCCC_DISPATCH_OP_LD      4 /* [4 rBASE offs rDEST] */
#define NCCC_DISPATCH_OP_ST      5 /* [5 rBASE offs rIN] */
#define NCCC_DISPATCH_OP_MOV     6 /* [6 rFROM rTO] */
#define NCCC_DISPATCH_OP_CALL    7 /* [7 rFUNC rINBUF rOUTBUF] */
#define NCCC_DISPATCH_OP_JMP     8 /* [8 imm] */
#define NCCC_DISPATCH_OP_JNZ     9 /* [9 rIN imm] */

NCCC_EXPORT void nccc_dispatch_0(const nccv64* op);
NCCC_EXPORT void nccc_dispatch_1(const nccv64* op, void* p0);
NCCC_EXPORT void nccc_dispatch_2(const nccv64* op, void* p0, void* p1);
NCCC_EXPORT void nccc_dispatch_3(const nccv64* op, void* p0, void* p1, 
                                 void* p2);
NCCC_EXPORT void nccc_dispatch_4(const nccv64* op, void* p0, void* p1, 
                                 void* p2, void* p3);
NCCC_EXPORT void nccc_dispatch_5(const nccv64* op, void* p0, void* p1, 
                                 void* p2, void* p3, void* p4);
NCCC_EXPORT void nccc_dispatch_6(const nccv64* op, void* p0, void* p1, 
                                 void* p2, void* p3, void* p4, void* p5);
NCCC_EXPORT void nccc_dispatch_7(const nccv64* op, void* p0, void* p1, 
                                 void* p2, void* p3, void* p4, void* p5, 
                                 void* p6);

#endif
