// destruirProcesoPar.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "ProcesoPar.h"
#include "internal_struct.h"


// forward declare internal
typedef struct ProcesoPar_t ProcesoPar_t;
extern void cleanup_procesopar(ProcesoPar_t* p);

Estado_t destruirProcesoPar(ProcesoPar handle){
    if(!handle) return E_INVALID_ARG;
    ProcesoPar_t* p = (ProcesoPar_t*)handle;
    cleanup_procesopar(p);
    return E_OK;
}
