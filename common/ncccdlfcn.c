#include <stdint.h>

#ifdef _WIN32
#include "win32-dlfcn.h"
#else
#include <dlfcn.h>
#endif

#include "nccv64.h"

void
dlfcn_open(const nccv64* in, nccv64* out){
    // [path] => [res ptr]
    void* handle;
    char* const path = in[0].ptr;
    handle = dlopen(path, RTLD_NOW);
    if(handle){
        out[0].s64 = 0;
    }else{
        out[0].s64 = -1;
    }
    out[1].uptr = (uintptr_t)handle;
}

void
dlfcn_get(const nccv64* in, nccv64* out){
    // [handle name] => [res ptr]
    void* handle = in[0].ptr;
    char* const symname = in[1].ptr;
    void* ptr;
    char* errs;

    (void)dlerror();
    ptr = dlsym(handle, symname);
    errs = dlerror();
    if(!errs){
        out[0].s64 = 0;
        out[1].ptr = ptr;
    }else{
        out[0].s64 = -1;
        out[1].s64 = -1;
    }
}

