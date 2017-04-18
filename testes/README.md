# Compilar hwloc no Windows

Para compilar programa que usa hwloc no Windows usando MinGW (MinGW já instalado):

- Baixar hwloc compilado para Windows de https://www.open-mpi.org/software/hwloc/v1.11/
(aparentemente, precisa ser a versão de
(32 bits)[https://www.open-mpi.org/software/hwloc/v1.11/downloads/hwloc-win32-build-1.11.6.zip] por causa do MinGW)
- Descompactar (em `<caminho>\hwloc`, por exemplo)
- Compilar

    g++ teste.c -o teste ^
    -I<caminho>\hwloc\include ^
    -L<caminho>\hwloc\lib -lhwloc

- Executar

    set path=<caminho>\hwloc\bin;%path%
    teste
