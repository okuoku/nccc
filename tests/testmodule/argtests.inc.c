#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

void test_argprint_s32_4(int32_t a, int32_t b, int32_t c, int32_t d, 
                         void* out_s, size_t outlen){
    /* FIXME: Use PRI */
    snprintf(out_s, outlen, "%d %d %d %d", a, b, c, d);
}

void test_argprint_s64_4(int64_t a, int64_t b, int64_t c, int64_t d, 
                         void* out_s, size_t outlen){
    /* FIXME: Use PRI */
    snprintf(out_s, outlen, "%" PRIx64 " %" PRIx64 " %" PRIx64 " %" PRIx64, a, b, c, d);
}

void test_argprint_f32_4(float a, float b, float c, float d, 
                         void* out_s, size_t outlen){
    snprintf(out_s, outlen, "%f %f %f %f", a, b, c, d);
}

void test_argprint_f64_4(double a, double b, double c, double d, 
                         void* out_s, size_t outlen){
    snprintf(out_s, outlen, "%lf %lf %lf %lf", a, b, c, d);
}

void test_argprint_mix_4(int32_t a, int64_t b, float c, double d, 
                         void* out_s, size_t outlen){
    /* FIXME: Use PRI */
    snprintf(out_s, outlen, "%d %" PRIx64 " %f %lf", a, b, c, d);
}

double test_argsum_mix_4_f64(int32_t a, int64_t b, float c, double d){
    return a + b + c + d;
}

