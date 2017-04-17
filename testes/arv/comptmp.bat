@echo off

rem Compila testes de tempo de execução
g++ -std=c++11 tst_tmp.cpp -o tmp -O3 %*
