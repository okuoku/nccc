#ifndef NCCC__NCCC_CORELIB_H
#define NCCC__NCCC_CORELIB_H

#include <stdint.h>
#include "nccv64.h"
#include "ncccexport.h"

/* Memory utils */
void nccc_memcpy(void* to, const void* from, uintptr_t size);

/* Dispatch */
#define NCCC_DISPATCH_OP_RETURN 0
#define NCCC_DISPATCH_OP_MOV    1 /* [1 inptr outptr] */
#define NCCC_DISPATCH_OP_MOVPTR 2 /* [2 reg outptr] */
#define NCCC_DISPATCH_OP_CALL   3 /* [3 func inptr outptr] */
#define NCCC_DISPATCH_OP_JMP    4 /* [4 opptr] */
#define NCCC_DISPATCH_OP_JNZ    5 /* [5 inptr opptr] */
NCCC_EXPORT void nccc_dispatch_0(const nccv64* op, nccv64* out);
NCCC_EXPORT void nccc_dispatch_1(const nccv64* op, nccv64* out, void* p0);
NCCC_EXPORT void nccc_dispatch_2(const nccv64* op, nccv64* out, void* p0, 
                                 void* p1);
NCCC_EXPORT void nccc_dispatch_3(const nccv64* op, nccv64* out, 
                                 void* p0, void* p1, void* p2);
NCCC_EXPORT void nccc_dispatch_4(const nccv64* op, nccv64* out, void* p0, 
                                 void* p1, void* p2, void* p3);
NCCC_EXPORT void nccc_dispatch_5(const nccv64* op, nccv64* out, 
                                 void* p0, void* p1, void* p2, void* p3, 
                                 void* p4);
NCCC_EXPORT void nccc_dispatch_6(const nccv64* op, nccv64* out, void* p0, 
                                 void* p1, void* p2, void* p3, void* p4, 
                                 void* p5);
NCCC_EXPORT void nccc_dispatch_7(const nccv64* op, nccv64* out, void* p0, 
                                 void* p1, void* p2, void* p3, void* p4, 
                                 void* p5, void* p6);

#endif
