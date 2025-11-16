// reader_thread.c
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ProcesoPar.h"
#include "internal_struct.h"


// forward declarations
typedef struct ProcesoPar_t ProcesoPar_t;
extern uint32_t read_be32(const unsigned char* b);

static void call_callback_safe(ProcesoPar_t* p, const char* buf, int len){
    if(p->callback){
        p->callback(buf, len, p->user_data);
    }
}

void reader_thread_start(ProcesoPar_t* p){
    unsigned char header[4];
    while(p->running){
        DWORD rd = 0;
        BOOL ok = ReadFile(p->hChildStd_OUT_Rd, header, 4, &rd, NULL);
        if(!ok || rd == 0){
            p->running = 0;
            break;
        }
        DWORD need = 4 - rd;
        while(need > 0){
            DWORD r2 = 0;
            ok = ReadFile(p->hChildStd_OUT_Rd, header + (4 - need), need, &r2, NULL);
            if(!ok || r2 == 0){ p->running = 0; break; }
            need -= r2;
        }
        if(!p->running) break;
        uint32_t len = read_be32(header);
        if(len == 0){
            call_callback_safe(p, NULL, 0);
            continue;
        }
        char* buf = (char*)malloc(len);
        if(!buf){
            DWORD toskip = len;
            char tmp[512];
            while(toskip > 0){
                DWORD r = 0;
                DWORD want = (toskip > sizeof(tmp)) ? sizeof(tmp) : toskip;
                ok = ReadFile(p->hChildStd_OUT_Rd, tmp, want, &r, NULL);
                if(!ok || r == 0){ p->running = 0; break; }
                toskip -= r;
            }
            if(!p->running) break;
            continue;
        }
        DWORD remaining = len;
        char* cur = buf;
        while(remaining > 0){
            DWORD r = 0;
            DWORD want = (remaining > 4096) ? 4096 : remaining;
            ok = ReadFile(p->hChildStd_OUT_Rd, cur, want, &r, NULL);
            if(!ok || r == 0){ free(buf); p->running = 0; break; }
            remaining -= r;
            cur += r;
        }
        if(!p->running){ break; }
        call_callback_safe(p, buf, (int)len);
        free(buf);
    }
}
