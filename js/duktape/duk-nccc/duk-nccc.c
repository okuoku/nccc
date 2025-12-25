#define _CRT_SECURE_NO_WARNINGS
// FIXME: We use global-stash[pointer] to keep backreference
#include <string.h>
#include <stdlib.h>
#include "duktape.h"

#include "nccv64.h"

#ifdef WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

static void
value_in(duk_context* ctx, char type, nccv64 vin){
    switch(type){
        case 'i':
            duk_push_int(ctx, vin.s32);
            break;
        case 'l':
            duk_push_number(ctx, vin.s64);
            break;
        case 'p':
            duk_push_pointer(ctx, vin.ptr);
            break;
        case 'f':
            duk_push_number(ctx, vin.f32);
            break;
        case 'd':
            duk_push_number(ctx, vin.f64);
            break;
        default:
            /* Unknown type */
            abort();
            break;
    }
}

static int // => bool
get_pointer(duk_context* ctx, duk_idx_t v, nccv64* value){
    if(duk_is_pointer(ctx, v)){
        value->ptr = duk_require_pointer(ctx, v);
        return 1;
    }else if(duk_is_buffer_data(ctx, v)){
        value->ptr = duk_get_buffer_data(ctx, v, NULL);
        return 1;
    }else if(duk_is_string(ctx, v)){
        value->ptr = (void*)duk_require_string(ctx, v);
        return 1;
    }else if(duk_is_object(ctx, v)){
        /* Pointer with finalizer */
        (void)duk_get_prop_index(ctx, v, 0);
        value->ptr = duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        if(value->ptr){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

static void
value_out(duk_context* ctx, nccv64* out, char type, duk_idx_t vin){
    int altfill = 0;
    nccv64 v;
    if(duk_is_null_or_undefined(ctx, vin)){
        altfill = 1;
        v.s64 = 0;
    }else if(duk_is_boolean(ctx, vin)){
        altfill = 1;
        v.s64 = duk_require_boolean(ctx, vin) ? 1 : 0;
    }

    /* numberlike */
    switch(type){
        case 'p':
            if(altfill){
                out->sptr = v.s64;
            }else if(get_pointer(ctx, vin, &v)){
                out->sptr = v.s64;
            }else{
                out->sptr = duk_require_number(ctx, vin);
            }
            break;
        case 'i':
            if(altfill){
                out->s32 = v.s64;
            }else if(get_pointer(ctx, vin, &v)){
                out->s32 = v.s64;
            }else{
                out->s32 = duk_require_number(ctx, vin);
            }
            break;
        case 'l':
            if(altfill){
                out->s64 = v.s64;
            }else if(get_pointer(ctx, vin, &v)){
                out->s64 = v.s64;
            }else{
                out->s64 = (int64_t)duk_require_number(ctx, vin);
            }
            break;
        case 'f':
            if(altfill){
                out->f32 = v.s64;
            }else{
                out->f32 = duk_require_number(ctx, vin);
            }
            break;
        case 'd':
            if(altfill){
                out->f64 = v.s64;
            }else{
                out->f64 = duk_require_number(ctx, vin);
            }
            break;
        default:
            abort();
            break;
    }
}

typedef void (*nccc_call_t)(const nccv64* in, nccv64* out);

struct finalizer_params_s {
    nccv64 dispatch;
    nccv64 ctx;
    nccv64 arg;
};

typedef struct finalizer_params_s finalizer_params_t;

static duk_ret_t
do_finalize(duk_context* ctx){
    nccc_call_t fn;
    nccv64 in0[2];
    nccv64 in1[2];
    void* v;
    finalizer_params_t* params;
    (void)duk_get_prop_index(ctx, 0, 1);
    params = duk_get_pointer(ctx, -1);
    (void)duk_get_prop_index(ctx, 0, 0);
    v = duk_get_pointer(ctx, -1);
    in0[0].ptr = params->arg.ptr;
    in0[1].ptr = v;
    if(params->dispatch.sptr){
        fn = (nccc_call_t)params->dispatch.ptr;
        in1[0] = params->ctx;
        in1[1].ptr = in0;
        fn(in1, NULL);
    }else{
        fn = (nccc_call_t)params->ctx.ptr;
        fn(in0, NULL);
    }
    free(params);

    return 0;
}

static duk_ret_t
wrap_pointer(duk_context* ctx){
    // [ptr dispatch ctx arg] => ptr
    //   [dispatch,ctx] = [arg ptr] => []
    finalizer_params_t* params;
    nccv64 ptr;
    params = malloc(sizeof(finalizer_params_t));
    if(!params){
        abort();
    }
    value_out(ctx, &ptr, 'p', 0);
    value_out(ctx, &params->dispatch, 'p', 1);
    value_out(ctx, &params->ctx, 'p', 2);
    value_out(ctx, &params->arg, 'p', 3);
    duk_push_object(ctx);
    duk_push_c_function(ctx, do_finalize, 1);
    duk_set_finalizer(ctx, -2);
    duk_push_pointer(ctx, ptr.ptr);
    duk_put_prop_index(ctx, -2, 0);
    duk_push_pointer(ctx, params);
    duk_put_prop_index(ctx, -2, 1);

    return 1;
}

struct cb_params_s {
    const char* intypes;
    const char* outtypes;
    size_t incount;
    size_t outcount;
    /* For nccc_call */
    nccv64 addr;
    nccv64 dispatch;
    /* For nccc_cb */
    duk_context* ctx;
};

typedef struct cb_params_s cb_params_t;

static duk_ret_t
destroy_cb_ctx(duk_context* ctx){
    // [params] => undefined
    nccv64 ptr;
    value_out(ctx, &ptr, 'p', 0);
    /* Remove params from Global stash */
    duk_push_global_stash(ctx);
    duk_del_prop_index(ctx, -1, ptr.uptr);
    free((cb_params_t*)ptr.ptr);
    return 0;
}

static duk_ret_t
nccc_call_trampoline(duk_context* ctx){
    int i;
    nccv64* inbuf;
    nccv64* outbuf;
    nccc_call_t fn;
    cb_params_t* params;
    duk_push_current_function(ctx);
    (void)duk_get_prop_index(ctx, -1, 0);
    params = duk_require_pointer(ctx, -1);

    if(params->dispatch.sptr){
        inbuf = alloca(sizeof(nccv64)*(params->incount+1));
    }else{
        inbuf = alloca(sizeof(nccv64)*params->incount);
    }
    outbuf = alloca(sizeof(nccv64)*params->outcount);
    /* Setup arguments */
    if(params->dispatch.sptr){
        inbuf[0] = params->addr;
        for(i=0;i!=params->incount;i++){
            value_out(ctx,&inbuf[i+1],params->intypes[i],i);
        }
    }else{
        for(i=0;i!=params->incount;i++){
            value_out(ctx,&inbuf[i],params->intypes[i],i);
        }
    }

    /* Call */
    if(params->dispatch.sptr){
        fn = (nccc_call_t)params->dispatch.ptr;
    }else{
        fn = (nccc_call_t)params->addr.ptr;
    }

    /* No need to handle exception here.
     * Duktape uses setjmp/longjmp for exceptions */

    fn(inbuf, outbuf);

    if(params->outcount == 0){
       return 0;
   }else if(params->outcount == 1){
       /* Push a value and return */
       value_in(ctx, params->outtypes[0], outbuf[0]);
       return 1;
   }else{ /* Return as an array */
       duk_push_array(ctx);
       for(i=0;i!=params->outcount;i++){
           value_in(ctx, params->outtypes[i], outbuf[i]);
           duk_put_prop_index(ctx, -2, i);
       }
       return 1;
   }
}

static void
free_cb_params(cb_params_t* p){
    free((void*)p->intypes);
    free((void*)p->outtypes);
    free(p);
}

static duk_ret_t
do_free_nccc_call(duk_context* ctx){
    cb_params_t* params;

    (void)duk_get_prop_index(ctx, -1, 0);
    params = duk_require_pointer(ctx, -1);
    free_cb_params(params);

    return 0;
}

static const char*
dukstrdup(duk_context* ctx, duk_idx_t v){
    size_t len;
    const char* buf;
    char* p;

    buf = duk_require_lstring(ctx, v, &len);
    p = malloc(len+1);
    if(!p){
        abort();
    }
    memcpy(p, buf, len);
    p[len] = 0;
    return (const char*)p;
}

static duk_ret_t
make_nccc_call(duk_context* ctx){
    // [debugstring, dispatch, addr, intypes, outtypes] => closure
    cb_params_t* params;

    // FIXME: debugstring

    params = malloc(sizeof(cb_params_t)); /* FIXME: Allocate this to stack first */
    if(!params){
        abort();
    }
    params->intypes = dukstrdup(ctx, 3);
    params->outtypes = dukstrdup(ctx, 4);
    (void)duk_require_lstring(ctx, 3, &params->incount);
    (void)duk_require_lstring(ctx, 4, &params->outcount);
    value_out(ctx, &params->dispatch, 'p', 1);
    value_out(ctx, &params->addr, 'p', 2);
    params->ctx = NULL;

    duk_push_c_function(ctx, nccc_call_trampoline, params->incount);
    duk_push_pointer(ctx, params);
    duk_put_prop_index(ctx, -2, 0);

    duk_push_c_function(ctx, do_free_nccc_call, 1);
    duk_set_finalizer(ctx, -2);

    return 1;
}

static void
nccc_cb_dispatcher(const nccv64* in, nccv64* out){
    // [params inaddr] => (dispatch)
    // Native to JavaScript bridge

    duk_context* ctx;
    cb_params_t* params = (cb_params_t*)in[0].ptr;
    const nccv64* in_next = (nccv64*)in[1].ptr;
    int i;
    char outtypes[33];
    int outcount;

    ctx = params->ctx;
    /* Function */
    duk_push_global_stash(ctx);
    (void)duk_get_prop_index(ctx, -1, in[0].uptr);
    if(!duk_is_function(ctx, -1)){
        abort();
    }

    /* Args */
    for(i=0;i!=params->incount;i++){
        value_in(ctx, params->intypes[i], in_next[i]);
    }

    // Capture ctx data so we can free ctx inside callback
    outcount = params->outcount;
    strncpy(outtypes, params->outtypes, 32);
    outtypes[32] = 0;

    duk_call(ctx, params->incount);
    
    // Parse and fill output
    if(outcount){
        if(duk_is_array(ctx, -1)){
            for(i=0;i!=outcount;i++){
                (void)duk_get_prop_index(ctx, -1, i);
                value_out(ctx, &out[i], outtypes[i], -1);
                duk_pop(ctx);
            }
        }else{
            value_out(ctx, &out[0], outtypes[0], -1);
        }
    }

    duk_pop(ctx);
    duk_pop(ctx); /* Global stash */
}

static duk_ret_t
make_nccc_cb(duk_context* ctx){
    // [cb, intypes, outtypes] => [dispatch ctx]
    //      __types:
    //         'i' : 32bit int
    //         'l' : 64bit int
    //         'p' : pointer
    //         'f' : float
    //         'd' : double

    cb_params_t* params;

    params = malloc(sizeof(cb_params_t)); /* FIXME: Allocate this to stack first */
    if(!params){
        abort();
    }
    params->intypes = dukstrdup(ctx, 1);
    params->outtypes = dukstrdup(ctx, 2);
    (void)duk_require_lstring(ctx, 1, &params->incount);
    (void)duk_require_lstring(ctx, 2, &params->outcount);
    params->dispatch.ptr = 0;
    params->addr.ptr = 0;
    params->ctx = ctx;

    /* retain callback */
    duk_push_global_stash(ctx);
    duk_dup(ctx, 0);
    (void)duk_put_prop_index(ctx, -2, (uintptr_t)params);

    /* Output */
    duk_push_array(ctx);
    duk_push_pointer(ctx, nccc_cb_dispatcher);
    (void)duk_put_prop_index(ctx, -2, 0);
    duk_push_pointer(ctx, params);
    (void)duk_put_prop_index(ctx, -2, 1);

    return 1;
}

#define EXPAND_CORELIB(x) \
    x(util_rawcall) \
    x(util_peek_u64) \
    x(util_peek_u32) \
    x(util_peek_u8) \
    x(util_peek_ptr) \
    x(util_poke_u64) \
    x(util_poke_u32) \
    x(util_malloc) \
    x(util_free) \
    x(util_ptraddr) \
    x(dlfcn_open) \
    x(dlfcn_get) \
    x(util_shortcircuit) \
    x(math_cos) \
    x(math_sin) \
    x(math_atan2) \
    x(math_sqrt) \
    x(math_pow) \
    x(math_fmodf)

#define ITR_PROTOTYPE(nam) \
    void nam(const uint64_t* in, uint64_t* out);

EXPAND_CORELIB(ITR_PROTOTYPE)

static void
set_corelib(duk_context* ctx, const char* name, void* sym){
    duk_push_pointer(ctx, sym);
    (void)duk_put_prop_string(ctx, -2, name);
}

static void /* push a value */
create_corelib(duk_context* ctx){
    duk_push_object(ctx);
#define ITR_SETCORELIB(sym) set_corelib(ctx, #sym, sym);
    EXPAND_CORELIB(ITR_SETCORELIB)
#undef ITR_SETCORELIB
}

static void
set_export(duk_context* ctx, const char* name, void* sym, int argc){
    duk_push_c_function(ctx, sym, argc);
    (void)duk_put_prop_string(ctx, -2, name);
}

void
dukregisternccc(duk_context* ctx){
    duk_push_global_object(ctx);
    duk_push_object(ctx);
#define REGISTER_EXPORT(sym,argc) set_export(ctx, #sym, sym, argc);
    REGISTER_EXPORT(make_nccc_cb,3);
    REGISTER_EXPORT(make_nccc_call,5);
    REGISTER_EXPORT(destroy_cb_ctx,1);
    REGISTER_EXPORT(wrap_pointer,4);

    create_corelib(ctx);
    (void)duk_put_prop_string(ctx, -2, "corelib");

    (void)duk_put_prop_string(ctx, -2, "NCCC");
    duk_pop(ctx);
}
