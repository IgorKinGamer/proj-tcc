@echo off

rem Graus com os quais rodará
for /f "tokens=*" %%g in (graus_testes.txt) ^
do (
	echo -------
	echo ^> tmp %* - %%~g
	tmp %* - %%~g
)
