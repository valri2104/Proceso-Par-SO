// test_client.c
// Example usage of the ProcesoPar library.
// Compile and run: (after building library and child)
// test_client.exe "examples\\child_echo.exe"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ProcesoPar.h"
#ifdef _WIN32
#include <windows.h>
#endif

void my_callback(const char* buf, int len, void* user_data){
    printf("[Callback] Received %d bytes: ", len);
    for(int i=0;i<len;i++){
        unsigned char c = (unsigned char)buf[i];
        if(c >= 32 && c < 127) putchar(c); else printf("\\x%02X", c);
    }
    printf("\\n");
}

int main(int argc, char** argv){
    if(argc < 2){
        printf("Usage: %s \"examples\\\\child_echo.exe\"\n", argv[0]);
        return 1;
    }
    ProcesoPar h = NULL;
    if(lanzarProcesoPar(argv[1], &h) != E_OK){
        printf("Error launching child\\n");
        return 1;
    }
    establecerFuncionDeEscucha(h, my_callback, NULL);

    const char* msg1 = "Hola Mundo!";
    printf("Sending message: %s\\n", msg1);
    enviarMensajeProcesoPar(h, msg1, (int)strlen(msg1));

    char bin[6] = {0x41,0x00,0x42,0x00,0x43,0xFF};
    printf("Sending binary message (6 bytes)\\n");
    enviarMensajeProcesoPar(h, bin, 6);

    #ifdef _WIN32
    Sleep(1500);
    #else
    sleep(1);
    #endif

    destruirProcesoPar(h);
    return 0;
}
