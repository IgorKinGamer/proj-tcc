@echo off

if "%saida_testes_tempo%"=="" set saida_testes_tempo=NUL

rem Graus com os quais rodar�
for /f "tokens=*" %%g in (graus_testes.txt) ^
do (
	for /f %%h in ('time /t') do (
		echo ^> tmp %* - %%~g  ^(In�cio: %%h^) )
	tmp %* - %%~g >> %saida_testes_tempo%
)
for /f %%h in ('time /t') do (
	echo ^(Final: %%h^) )