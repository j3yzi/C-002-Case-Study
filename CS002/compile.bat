@echo off
:: Compile script for CS002 project

:: Create bin directory if it doesn't exist
if not exist bin mkdir bin

:: Define compiler and flags
set CC=C:\msys64\ucrt64\bin\gcc.exe
set CFLAGS=-g -Wall -I./include

:: Build the project
echo Building project...
%CC% %CFLAGS% ^
    src/main.c ^
    src/lib/appwnd.c ^
    src/lib/lismgr.c ^
    src/lib/lisops.c ^
    src/lib/menu.c ^
    src/lib/validation.c ^
    src/ui/menuio.c ^
    -o bin/project_cs002.exe

:: Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo Build successful! Executable created at: bin/project_cs002.exe
    
    :: Optionally run the program
    :: Remove the "rem" from the next line to run the program after successful compilation
    rem bin\project_cs002.exe
) else (
    echo Build failed with error code: %ERRORLEVEL%
)

pause
