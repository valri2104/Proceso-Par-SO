# Makefile for MSYS2 / MinGW (Windows)
CC = gcc
CFLAGS = -O2 -Wall -I../include
SRCS = lanzarProcesoPar.c establecerFuncionDeEscucha.c enviarMensajeProcesoPar.c destruirProcesoPar.c reader_thread.c internal_impl.c
OBJS = $(SRCS:.c=.o)
LIBNAME = libprocesopar.a
AR = ar

all: $(LIBNAME) examples

$(LIBNAME): $(OBJS)
	$(AR) rcs $(LIBNAME) $(OBJS)

%.o: ../src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

examples:
	$(CC) -o ../examples/child_echo.exe ../examples/child_echo.c
	$(CC) -I../include -o ../examples/test_client.exe ../examples/test_client.c -L. -lprocesopar


clean:
	rm -f *.o $(LIBNAME) ../examples/*.exe
