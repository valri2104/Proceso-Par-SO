// establecerFuncionDeEscucha.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include "ProcesoPar.h"
#include "internal_struct.h"


typedef struct ProcesoPar_t ProcesoPar_t;
extern void reader_thread_start(ProcesoPar_t* p);

Estado_t establecerFuncionDeEscucha(ProcesoPar handle, ProcesoPar_Callback cb, void* user_data){
    if(!handle) return E_INVALID_ARG;
    ProcesoPar_t* p = (ProcesoPar_t*)handle;
    p->callback = cb;
    p->user_data = user_data;
    // If reader thread not started, start it
    if(!p->hThreadReader){
        uintptr_t th = _beginthread((void(*)(void*))reader_thread_start, 0, p);
        if(th == (uintptr_t)-1) return E_ERROR;
        p->hThreadReader = (HANDLE)th;
    }
    return E_OK;
}
