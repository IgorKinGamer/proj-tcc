# Compila testes de tempo de execução
g++ -std=c++11 tst_tmp.cpp -o tmp -O3 \
	$(pkg-config --cflags hwloc) \
	$(pkg-config --libs hwloc)
