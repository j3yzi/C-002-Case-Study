@echo off
echo Compiling test_validation.c...
C:\msys64\ucrt64\bin\gcc.exe -fdiagnostics-color=always -g ^
-I../include ^
-I../../appcontext ^
test_validation.c lib/validation.c ^
-o test_validation.exe

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running test_validation.exe...
    echo.
    test_validation.exe
) else (
    echo Compilation failed!
)
pause
