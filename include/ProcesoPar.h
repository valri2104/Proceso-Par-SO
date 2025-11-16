#ifndef PROCESOPAR_H
#define PROCESOPAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Opaque handle to the proceso par
typedef struct ProcesoPar_t* ProcesoPar;

// Estado resultante de las operaciones
typedef enum {
    E_OK = 0,
    E_ERROR = 1,
    E_INVALID_ARG = 2,
    E_NO_MEMORY = 3,
    E_IO = 4,
    E_NOT_RUNNING = 5,
    E_ALREADY_RUNNING = 6
} Estado_t;

// Callback type invoked when a message is received from the child process.
// buffer may contain binary data and is NOT null-terminated. length is the number of bytes.
typedef void (*ProcesoPar_Callback)(const char* buffer, int length, void* user_data);

// Lanzar un proceso hijo y establecer la comunicación mediante pipes.
// `program` es el path al ejecutable hijo (por ejemplo: examples\\child_echo.exe).
// `args` puede ser NULL o un arreglo estilo argv (NULL-terminated). Por simplicidad en Windows,
// se pasa un string con los argumentos (como CreateProcess requiere).
// `out_handle` devuelve el handle del ProcesoPar creado.
// Retorna E_OK o un Estado_t de error.
Estado_t lanzarProcesoPar(const char* program_with_args, ProcesoPar* out_handle);

// Destruir el proceso par: cierra pipes, termina hilo lector y espera al hijo.
// Retorna E_OK o error.
Estado_t destruirProcesoPar(ProcesoPar handle);

// Enviar mensaje al proceso hijo. El mensaje puede contener bytes nulos.
// Implementación usa un header de 4 bytes (big-endian) con la longitud del payload seguido del payload.
// Bloqueante: intenta escribir todo hasta completarlo o retornar error si el pipe está roto.
Estado_t enviarMensajeProcesoPar(ProcesoPar handle, const char* buffer, int length);

// Establecer (o reemplazar) la función de escucha que se invocará cuando lleguen mensajes.
// `user_data` será pasado al callback. Puede pasar NULL.
// Retorna E_OK.
Estado_t establecerFuncionDeEscucha(ProcesoPar handle, ProcesoPar_Callback cb, void* user_data);

#ifdef __cplusplus
}
#endif

#endif // PROCESOPAR_H
