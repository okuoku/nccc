#include <stdint.h>
#include <float.h>
#include <math.h>

/* Pointer */
void* test_retptr_nil(void){
    return 0;
}

void* test_retptr_msg(void){
    return "This is a test message";
}

void* test_retptr_nilstr(void){
    return "";
}

void* test_retptr_u32seq(void){
    static const uint32_t t[] = {1, 2, 3, 4};
    /* FIXME: Deconst */
    return (void*)t;
}

void* test_retptr_u64seq(void){
    static const uint64_t t[] = {1, 2, 3, 4};
    /* FIXME: Deconst */
    return (void*)t;
}

/* Integral */

uint32_t test_ret10_u32(void){
    return 10;
}

int32_t test_ret10_s32(void){
    return 10;
}

int32_t test_retm10_s32(void){
    return -10;
}

uint64_t test_ret10_u64(void){
    return 10;
}

int64_t test_ret10_s64(void){
    return 10;
}

int64_t test_retm10_s64(void){
    return -10;
}

uint32_t test_retMAX_u32(void){
    return UINT32_MAX; /* 4294967295 */
}

int32_t test_retMAX_s32(void){
    return INT32_MAX; /* 2147483647 */
}

int32_t test_retMIN_s32(void){
    return INT32_MIN; /* -2147483648 */
}

uint64_t test_retMAX_u64(void){
    return UINT64_MAX; /* 18446744073709551615 */
}

int64_t test_retMAX_s64(void){
    return INT64_MAX; /* 9223372036854775807 */
}

int64_t test_retMIN_s64(void){
    return INT64_MIN; /* -9223372036854775808 */
}

uintptr_t test_retMAX_uptr(void){
    return UINTPTR_MAX;
}

intptr_t test_retMAX_sptr(void){
    return INTPTR_MAX;
}

intptr_t test_retMIN_sptr(void){
    return INTPTR_MIN;
}

/* Floating point */

float test_ret10_f32(void){
    return 10.0;
}

float test_retm10_f32(void){
    return -10.0;
}

double test_ret10_f64(void){
    return 10.0;
}

double test_retm10_f64(void){
    return -10.0;
}

float test_retMAX_f32(void){
    return FLT_MAX;
}

float test_retMIN_f32(void){
    return FLT_MIN;
}

double test_retMAX_f64(void){
    return DBL_MAX;
}

double test_retMIN_f64(void){
    return DBL_MIN;
}

float test_retHUGE_f32(void){
    return INFINITY;
}

double test_retHUGE_f64(void){
    return HUGE_VAL;
}

float test_retNAN_f32(void){
    return nan("");
}

double test_retNAN_f64(void){
    return nanl("");
}

float test_retmzero_f32(void){
    return -0.0;
}

double test_retmzero_f64(void){
    return -0.0;
}
