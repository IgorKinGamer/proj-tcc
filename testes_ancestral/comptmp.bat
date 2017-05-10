@echo off

rem Compila testes de tempo de execução
g++ -std=c++11 tst_tmp\tst_tmp.cpp -o tmp -O3 ^
	-I%hwloc%\include ^
	-L%hwloc%\lib -lhwloc ^
	%*