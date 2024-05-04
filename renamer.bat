:: application purpose only, only rename e field files with specific way 
@echo off
setlocal enableextensions enabledelayedexpansion 
mkdir ".\final"
for /f "tokens=*" %%s in ('dir /b *.efe') do (
	for /f "tokens=1,2 delims=ABCDEFGHIJKLMNOPQRSTUVWYZabcdefghijklmnoprqstuvwyz" %%A in ("%%~ns") do (
			setlocal
			set /a rem = %%A %% 2
			if !rem! == 0 (	
				set /a div = %%A / 2 
				echo new number is !div!
			move /Y "%%s" ".\final\Efield!div!.efe"
					)
			endlocal  
	)
)
