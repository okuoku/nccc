/* Main */

#include <stdio.h>
#include <stdlib.h>
#include "duktape.h"

#include "duk_print_alert.h"

#define BOOTSTRAP_FILE (SCRIPTROOT "/bootstrap.js")

void dukregisternccc(duk_context* ctx);

void
dukdebugwrite(long level, const char* file, long line, const char* func, const char* msg){
    fprintf(stderr, "D%ld %s:%d (%s): %s\n", level, file, line, func, msg);
}

static duk_ret_t
readfile(duk_context* ctx, int readasbinary){
    long siz;
    FILE* fp;
    char* buf;
    void* p;
    const char* filename;
    filename = duk_require_string(ctx, 0);
    fp = fopen(filename, "r");
    if(!fp){
        duk_push_boolean(ctx, 0);
        return 1;
    }
    fseek(fp,0,SEEK_END);
    siz = ftell(fp);
    fseek(fp,0,SEEK_SET);
    buf = malloc(siz);
    fread(buf,1,siz,fp);

    if(readasbinary){
        p = duk_push_fixed_buffer(ctx, siz);
        memcpy(p, buf, siz);
    }else{
        duk_push_lstring(ctx, buf, siz);
    }
    free(buf);
    return 1;
}

static duk_ret_t
readtext(duk_context* ctx){
    return readfile(ctx, 0);
}

static duk_ret_t
readbinary(duk_context* ctx){
    return readfile(ctx, 1);
}

/* Uint8Array.prototype.copyWithin() polyfill */
static duk_ret_t
copywithin_uint8(duk_context* ctx){
    // [target start (end)] => this
    const double target = duk_require_number(ctx, 0);
    const double start = duk_require_number(ctx, 1);
    const double end = duk_require_number(ctx, 2);
    const size_t len = end - start;
    char* p;
    size_t buflen;
    duk_push_this(ctx);
    p = duk_require_buffer_data(ctx, -1, &buflen);
    if(target + len > buflen){
        fprintf(stderr, "????");
        abort();
    }
    // FIXME: memmove?
    memcpy(p + (size_t)target, p + (size_t)start, (size_t)len);

    return 1;
}

/* clz32 */
static duk_ret_t
clz32(duk_context* ctx){
    const double d = duk_require_number(ctx, 0);
    const uint32_t u = d;

    uint32_t ret;
    if(u == 0){
        ret = 32;
    }else{
        ret = __builtin_clz(u);
    }
    duk_push_number(ctx, ret);
    return 1;
}

static void
dukload(duk_context* ctx, const char* filename, int flags){
    long siz;
    FILE* fp;
    char* buf;
    fp = fopen(filename, "r");
    fseek(fp,0,SEEK_END);
    siz = ftell(fp);
    fseek(fp,0,SEEK_SET);
    buf = malloc(siz+1);
    fread(buf,1,siz,fp);
    buf[siz] = 0;

    duk_push_string(ctx, filename);
    duk_compile_string_filename(ctx, flags, buf);
    free(buf);
    duk_call(ctx, 0);
    duk_pop(ctx);
}

int main(int argc, char *argv[]) {
    duk_context *ctx = duk_create_heap_default();
    duk_print_alert_init(ctx, 0);
    dukregisternccc(ctx);
    duk_eval_string(ctx, "print(JSON.stringify(NCCC));");
    duk_eval_string(ctx, "print(NCCC.corelib.util_rawcall);");
    duk_push_global_object(ctx);
    duk_push_object(ctx);
    duk_push_c_function(ctx, readbinary, 1);
    (void)duk_put_prop_string(ctx, -2, "readbinary");
    duk_push_c_function(ctx, readtext, 1);
    (void)duk_put_prop_string(ctx, -2, "readtext");
    duk_push_c_function(ctx, copywithin_uint8, 3);
    (void)duk_put_prop_string(ctx, -2, "copywithin_uint8");
    duk_push_c_function(ctx, clz32, 1);
    (void)duk_put_prop_string(ctx, -2, "clz32");
    (void)duk_put_prop_string(ctx, -2, "BOOTSTRAP");
    (void)duk_put_prop_string(ctx, -1, "global");
    duk_pop(ctx);
    dukload(ctx, BOOTSTRAP_FILE, DUK_COMPILE_EVAL);
    duk_destroy_heap(ctx);
    return 0;
}
