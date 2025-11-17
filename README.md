# Proyecto: Biblioteca ProcesoPar — Comunicación bidireccional entre Procesos en Windows
*Práctica de Sistemas Operativos — Comunicación por Tuberías (Pipes)*

## Descripción general

Este proyecto implementa una biblioteca en C llamada **ProcesoPar**, diseñada para permitir la comunicación **bidireccional** entre dos procesos en Windows mediante **tuberías anónimas (pipes)**.

La biblioteca permite:

- Lanzar un proceso hijo con entrada y salida redirigida.
- Enviar mensajes desde el proceso padre al hijo.
- Recibir mensajes enviados por el hijo a través de un **hilo lector**.
- Definir un **callback** llamado automáticamente cuando llegan datos nuevos.
- Manejar mensajes binarios o texto con longitud variable.

La intención es crear un mecanismo simple de *IPC* (Inter-Process Communication) basado en la API de Windows.

---

## Características principales

### ✔ Comunicación bidireccional mediante Pipes
El padre y el hijo se comunican mediante dos tuberías independientes:

- Pipe 1 → Padre → Hijo  
- Pipe 2 → Hijo → Padre  

### Protocolo basado en longitud

Cada mensaje tiene el formato:

```
[ 4 bytes big-endian | payload ]
```

Esto permite enviar:

- texto  
- binario  
- datos arbitrarios  
- mensajes con bytes nulos  

### Hilo lector independiente

Un hilo separado lee continuamente la salida estándar del hijo y llama al callback.

### Callback configurable

El usuario puede registrar una función personalizada para manejar los mensajes entrantes.

---

## Arquitectura del Proyecto

```
procesopar/
├── include/
│   ├── ProcesoPar.h
│   └── internal_struct.h
├── src/
│   ├── lanzarProcesoPar.c
│   ├── enviarMensajeProcesoPar.c
│   ├── establecerFuncionDeEscucha.c
│   ├── destruirProcesoPar.c
│   ├── reader_thread.c
│   └── internal_impl.c
├── windows/
│   └── Makefile
└── examples/
    ├── child_echo.c
    └── test_client.c
```

---

## Cómo funciona la comunicación

### Diagrama

```
 ┌─────────────────────┐            ┌─────────────────────┐
 │   test_client.exe    │            │   child_echo.exe     │
 │   (Proceso padre)    │            │   (Proceso hijo)     │
 └───────────┬──────────┘            └──────────┬───────────┘
             │                                    │
             │ WriteFile()                        │
             ├──────────────► STDIN (pipe) ───────┘
             │                                    │
             │                                    │ Procesa mensaje
             │                                    │
             │ ◄──────────── STDOUT (pipe) ───────┤
             │        ReadFile() en un hilo       │
             │        callback(msg, len)          │
```

---

## Cómo compilar en Windows (MSYS2 + MinGW64)

### 1. Instalar MSYS2

https://www.msys2.org/

Abrir **MSYS2 MinGW 64-bit**.

### 2. Instalar GCC y make

```
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

### 3. Compilar

```
cd windows
make
```

Esto genera:

- libprocesopar.a  
- examples/child_echo.exe  
- examples/test_client.exe  

---

## Cómo probar

Ejecutar:

```
../examples/test_client.exe "../examples/child_echo.exe"
```

Salida esperada:

```
Sending message: hola mundo
[Callback] Received 15 bytes: ECHO:hola mundo

Sending binary message (6 bytes)
[Callback] Received 11 bytes: ECHO:A

```

## API ProcesoPar

### Lanzar proceso

```c
ProcesoPar_t* lanzarProcesoPar(const char* ruta);
```

### Enviar mensaje

```c
int enviarMensajeProcesoPar(ProcesoPar_t* p, const void* msg, uint32_t len);
```

### Registrar callback

```c
void establecerFuncionDeEscucha(ProcesoPar_t* p,
                                ProcesoPar_Callback f,
                                void* user_data);
```

### Destruir proceso

```c
void destruirProcesoPar(ProcesoPar_t* p);
```

---

## Ejemplo de callback

```c
void mi_callback(const void* msg, uint32_t len, void* ud) {
    printf("[Callback] Llegaron %u bytes: %.*s
", len, len, (const char*)msg);
}
```

---

## Limpieza

```
make clean
```

---

## Estado del Proyecto

| Requisito | Estado |
|----------|--------|
| Pipes | ✔ |
| Protocolo big-endian | ✔ |
| Hilo lector | ✔ |
| Callback | ✔ |
| Makefile | ✔ |
| Librería estática | ✔ |
| Ejemplos | ✔ |
| Proyecto funcional | 🎉 COMPLETO |

---

## Autor

**Valeria Cardona Urrea**  
Materia: Sistemas Operativos  
