@echo off

rem Argumentos com os quais rodará
for %%g in (
	"8 1 1 6 1 1 1"
	"4 4 1 3 2 1 1 1"
	"2 1 1 8 1 1 1 2"
	"2 2 1 5 1 1 1 1 8"
	"1 4 1 1 9 2 1 1 4"
) do (
	echo -------
	echo ^> tmp - %%~g
	tmp - %%~g
)