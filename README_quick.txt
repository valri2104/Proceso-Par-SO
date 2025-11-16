Instrucciones rápidas (Windows MSYS2 / MinGW):

1. Abre MSYS2 MinGW shell.
2. Navega a windows/:
   cd procesopar_windows/windows
3. Ejecuta make:
   make
   (Esto compilará la librería y los ejemplos.)
4. Ejecuta el ejemplo:
   ../examples/test_client.exe "../examples/child_echo.exe"

Nota: paths en Windows usan \\ en la invocación de CreateProcess - aquí pasamos el argumento completo
como un único string al ejecutar test_client.exe.
