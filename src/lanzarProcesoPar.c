// lanzarProcesoPar.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProcesoPar.h"
#include "internal_struct.h"


// forward declare internal structure
typedef struct ProcesoPar_t ProcesoPar_t;
extern ProcesoPar_t* create_procesopar();

Estado_t lanzarProcesoPar(const char* program_with_args, ProcesoPar* out_handle){
    if(!program_with_args || !out_handle) return E_INVALID_ARG;
    ProcesoPar_t* p = create_procesopar();
    if(!p) return E_NO_MEMORY;

    // Create pipes
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE childStd_IN_Rd = NULL;
    HANDLE childStd_IN_Wr = NULL;
    HANDLE childStd_OUT_Rd = NULL;
    HANDLE childStd_OUT_Wr = NULL;

    if(!CreatePipe(&childStd_OUT_Rd, &childStd_OUT_Wr, &saAttr, 0)){
        return E_IO;
    }
    if(!SetHandleInformation(childStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)){
        // continue but report later
    }

    if(!CreatePipe(&childStd_IN_Rd, &childStd_IN_Wr, &saAttr, 0)){
        CloseHandle(childStd_OUT_Rd);
        CloseHandle(childStd_OUT_Wr);
        return E_IO;
    }
    if(!SetHandleInformation(childStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)){
        // continue
    }

    // Create child process
    STARTUPINFOA siStartInfo;
    PROCESS_INFORMATION piProcInfo;
    ZeroMemory( &siStartInfo, sizeof(siStartInfo) );
    siStartInfo.cb = sizeof(siStartInfo);
    siStartInfo.hStdError = childStd_OUT_Wr;
    siStartInfo.hStdOutput = childStd_OUT_Wr;
    siStartInfo.hStdInput = childStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory( &piProcInfo, sizeof(piProcInfo) );

    BOOL ok = CreateProcessA(
        NULL,
        (LPSTR)program_with_args,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &siStartInfo,
        &piProcInfo
    );

    if(!ok){
        CloseHandle(childStd_OUT_Rd);
        CloseHandle(childStd_OUT_Wr);
        CloseHandle(childStd_IN_Rd);
        CloseHandle(childStd_IN_Wr);
        free(p);
        return E_IO;
    }

    // Close handles not needed by parent
    CloseHandle(childStd_OUT_Wr);
    CloseHandle(childStd_IN_Rd);

    p->hChildProcess = piProcInfo.hProcess;
    CloseHandle(piProcInfo.hThread);

    p->hChildStd_IN_Wr = childStd_IN_Wr;
    p->hChildStd_OUT_Rd = childStd_OUT_Rd;
    p->running = 1;
    p->callback = NULL;
    p->user_data = NULL;

    *out_handle = p;
    return E_OK;
}
