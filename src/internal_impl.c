// internal_impl.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "ProcesoPar.h"
#include "internal_struct.h"


void write_be32(unsigned char* b, unsigned int v){
    b[0] = (v >> 24) & 0xFF;
    b[1] = (v >> 16) & 0xFF;
    b[2] = (v >> 8) & 0xFF;
    b[3] = (v >> 0) & 0xFF;
}
unsigned int read_be32(const unsigned char* b){
    return ((unsigned int)b[0] << 24) | ((unsigned int)b[1] << 16) | ((unsigned int)b[2] << 8) | (unsigned int)b[3];
}

ProcesoPar_t* create_procesopar(){
    ProcesoPar_t* p = (ProcesoPar_t*)malloc(sizeof(ProcesoPar_t));
    if(!p) return NULL;
    p->hChildProcess = NULL;
    p->hChildStd_IN_Wr = NULL;
    p->hChildStd_OUT_Rd = NULL;
    p->hThreadReader = NULL;
    p->running = 0;
    p->callback = NULL;
    p->user_data = NULL;
    return p;
}

void cleanup_procesopar(ProcesoPar_t* p){
    if(!p) return;
    p->running = 0;
    if(p->hChildStd_IN_Wr){
        CloseHandle(p->hChildStd_IN_Wr);
        p->hChildStd_IN_Wr = NULL;
    }
    if(p->hChildStd_OUT_Rd){
        CloseHandle(p->hChildStd_OUT_Rd);
        p->hChildStd_OUT_Rd = NULL;
    }
    if(p->hChildProcess){
        WaitForSingleObject(p->hChildProcess, 2000);
        CloseHandle(p->hChildProcess);
        p->hChildProcess = NULL;
    }
    free(p);
}
