@echo off

rem Compila testes de tempo de execução
g++ -std=c++11 -o tmp -O3 ^
	-I%hwloc%\include tst_tmp.cpp ^
	-L%hwloc%\lib -lhwloc ^
	%*