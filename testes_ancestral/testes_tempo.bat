@echo off

rem Graus com os quais rodar�
for /f "tokens=*" %%g in (graus_testes.txt) ^
do (
	echo -------
	echo ^> tmp %* - %%~g
	tmp %* - %%~g
)
