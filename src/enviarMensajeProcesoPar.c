// enviarMensajeProcesoPar.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "ProcesoPar.h"
#include "internal_struct.h"


// forward declaration
typedef struct ProcesoPar_t ProcesoPar_t;
extern void write_be32(unsigned char* b, uint32_t v);

Estado_t enviarMensajeProcesoPar(ProcesoPar handle, const char* buffer, int length){
    if(!handle) return E_INVALID_ARG;
    if(length < 0) return E_INVALID_ARG;
    ProcesoPar_t* p = (ProcesoPar_t*)handle;
    if(!p->running) return E_NOT_RUNNING;

    // construct header + payload
    uint32_t len = (uint32_t)length;
    unsigned char header[4];
    write_be32(header, len);

    DWORD written;
    BOOL ok = WriteFile(p->hChildStd_IN_Wr, header, 4, &written, NULL);
    if(!ok || written != 4) return E_IO;

    if(len > 0){
        DWORD toWrite = len;
        const char* ptr = buffer;
        while(toWrite > 0){
            DWORD chunk = (toWrite > 4096) ? 4096 : toWrite;
            DWORD w = 0;
            ok = WriteFile(p->hChildStd_IN_Wr, ptr, chunk, &w, NULL);
            if(!ok) return E_IO;
            toWrite -= w;
            ptr += w;
            if(w == 0) return E_IO;
        }
    }
    return E_OK;
}
