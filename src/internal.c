// internal.c
// Shared internal definitions for the Windows implementation.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ProcesoPar.h"
#include "internal_struct.h"


typedef struct ProcesoPar_t {
    HANDLE hChildProcess;
    HANDLE hChildStd_IN_Wr;   // parent -> child (write)
    HANDLE hChildStd_OUT_Rd;  // child -> parent (read)
    HANDLE hThreadReader;
    volatile int running;
    ProcesoPar_Callback callback;
    void* user_data;
} ProcesoPar_t;

// Helpers for big-endian int32
static void write_be32(unsigned char* b, uint32_t v){
    b[0] = (v >> 24) & 0xFF;
    b[1] = (v >> 16) & 0xFF;
    b[2] = (v >> 8) & 0xFF;
    b[3] = (v >> 0) & 0xFF;
}
static uint32_t read_be32(const unsigned char* b){
    return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | (uint32_t)b[3];
}
