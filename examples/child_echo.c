// child_echo.c
// Simple child program that reads messages with 4-byte big-endian length header,
// then echoes back the payload prefixed with "ECHO:" (demonstrates binary-safe messages).

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


static uint32_t read_be32(unsigned char* b){
    return ((uint32_t)b[0]<<24) | ((uint32_t)b[1]<<16) | ((uint32_t)b[2]<<8) | b[3];
}
static void write_be32(unsigned char* b, uint32_t v){
    b[0] = (v>>24)&0xFF; b[1]=(v>>16)&0xFF; b[2]=(v>>8)&0xFF; b[3]=v&0xFF;
}

int main(int argc, char** argv){
    unsigned char hdr[4];
    while(1){
        size_t r = fread(hdr,1,4,stdin);
        if(r==0) return 0;
        if(r<4){
            size_t need = 4 - r;
            size_t r2 = fread(hdr+r,1,need,stdin);
            if(r2 != need) return 0;
        }
        uint32_t len = read_be32(hdr);
        unsigned char* buf = NULL;
        if(len>0){
            buf = (unsigned char*)malloc(len);
            size_t got = fread(buf,1,len,stdin);
            if(got != len){
                free(buf);
                return 0;
            }
        }
        const char* pre = "ECHO:";
        uint32_t outlen = (uint32_t)(strlen(pre) + len);
        unsigned char oh[4];
        write_be32(oh, outlen);
        fwrite(oh,1,4,stdout);
        fwrite(pre,1,strlen(pre),stdout);
        if(len>0){
            fwrite(buf,1,len,stdout);
            free(buf);
        }
        fflush(stdout);
    }
    return 0;
}
