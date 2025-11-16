#ifndef INTERNAL_STRUCT_H
#define INTERNAL_STRUCT_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ProcesoPar.h"


// Estructura interna REAL
typedef struct ProcesoPar_t {
    HANDLE hChildProcess;
    HANDLE hChildStd_IN_Wr;
    HANDLE hChildStd_OUT_Rd;
    HANDLE hThreadReader;
    volatile int running;
    ProcesoPar_Callback callback;
    void* user_data;
} ProcesoPar_t;

#endif
