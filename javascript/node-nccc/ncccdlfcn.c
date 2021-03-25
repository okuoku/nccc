#include <stdint.h>

#ifdef _WIN32
#include "win32-dlfcn.h"
#else
#include <dlfcn.h>
#endif

void
dlfcn_open(const uint64_t* in, uint64_t* out){
    // [path] => [res ptr]
    void* handle;
    char* const path = (char*)in[0];
    handle = dlopen(path, RTLD_NOW);
    if(handle){
        out[0] = 0;
    }else{
        out[0] = -1;
    }
    out[1] = (uintptr_t)handle;
}

void
dlfcn_get(const uint64_t* in, uint64_t* out){
    // [handle name] => [res ptr]
    void* handle = (uintptr_t)in[0];
    char* const symname = (uintptr_t)in[1];
    void* ptr;
    char* errs;

    (void)dlerror();
    ptr = dlsym(handle, symname);
    errs = dlerror();
    if(!errs){
        out[0] = 0;
        out[1] = (uintptr_t)ptr;
    }else{
        out[0] = -1;
        out[1] = -1;
    }
}

